
#include "Loom_Translator.h"
#include "Loom_Macros.h"




// ================================================================
// ===            CONVERSION BETWEEN BUNDLE FORMATS             ===
// ================================================================


void aux_convert_bundle_structure_to_single(OSCBundle *bndl, OSCBundle *outBndl)
{
	char address[50];
	bndl->getOSCMessage(0)->getAddress(address);
	snprintf(address, strrchr(address,'/')-address+1, "%s", address);
	sprintf(address, "%s%s", address, "/data");
	OSCMessage newMsg = OSCMessage(address);
	OSCMessage* msg;     // Temporarily hold message of bundle
	
	// Copy data of messages into new bundle
	for (int i = 0; i < bndl->size(); i++) {

		msg = bndl->getOSCMessage(i); 	// Get ith messsage

		// If the message only has one value
		if (msg->size() == 1) {
			// Get ith messsage key and add to new message
			char tmpBuf[50], keyBuf[50];
			msg->getAddress(tmpBuf);
			strncpy(keyBuf, strrchr(tmpBuf,'/')+1, 49);
			newMsg.add(keyBuf);

			// Get ith message data and add to new message
			switch (msg->getType(0)) {
				case 'f': newMsg.add(msg->getFloat(0));	break;
				case 'i': newMsg.add(msg->getInt(0));	break;
				case 's': char buf[256];  msg->getString(0, buf, 256);  newMsg.add(buf);  break;
				default: LOOM_DEBUG_Println("Unsupported data data_type.");
			}

		// If the message has multiple keys and values
		} else if (msg->size() > 1) { 

			char addr_buf[50], addr_end[20];
			osc_extract_header_section(msg, 5, addr_end);
			osc_extract_header_from_section(msg, 3, addr_buf); 

			// If in multiplexer format
			if ( strcmp("data", addr_end) == 0) {
				addr_buf[ strlen((char*)addr_buf)-5 ] = '\0';
			} 

			for (int i = 0; i < msg->size(); i+=2) {
				newMsg.add( ( String(addr_buf+1) + "/" + get_data_value(msg, i) ).c_str());
				switch (msg->getType(i+1)) {
					case 'f': newMsg.add(msg->getFloat(i+1));	break;
					case 'i': newMsg.add(msg->getInt(i+1));	break;
					case 's': char buf[256];  msg->getString(i+1, buf, 256);  newMsg.add(buf);  break;
					default: LOOM_DEBUG_Println("Unsupported data data_type.");
				}
			}

		} // of if-else
	} // of for
	outBndl->add(newMsg);
}



// This function should support the multiplexer complex bundles as well

void aux_convert_bundle_structure_to_multi(OSCBundle *bndl, OSCBundle *outBndl)
{
	OSCMessage *msg = bndl->getOSCMessage(0); 
	OSCMessage tmpMsg;
	char address[50], newAddress[50], keyBuf[50];

	// Save old packet header address
	msg->getAddress(address);

	// Copy flat data into messages of '/some/address/key value' format
	for (int i = 1; i < msg->size(); i+=2) {
		switch (msg->getType(i)) {
			case 'i': tmpMsg.add(msg->getInt(i));	break;
			case 'f': tmpMsg.add(msg->getFloat(i));	break;
			case 's': char buf[256];  msg->getString(i, buf, 256);  tmpMsg.add(buf);  break;
			default: LOOM_DEBUG_Println("Unsupported data data_type.");
		}

		// Add message as /address/key value
		snprintf(newAddress, strrchr(address,'/')-address+1, "%s", address);
		msg->getString(i-1, keyBuf, 50);
		sprintf(newAddress, "%s%s%s", newAddress, "/", keyBuf);
		tmpMsg.setAddress(newAddress);

		outBndl->add(tmpMsg);
		tmpMsg.empty();	
	} 
}

// --- CONVERT BUNDLE STRUCTURE ---
//
// This function can convert a bundle in either 
// single or mulitple message format to the other
//
// Single Message Format:
//   An OSC bundle with a single message, in the format:
// 		Bundle Size: 1
//		Address 1: /LOOM/Device7/data "abc" 1 "def" 2.0 "ghi" 3.2 
//
// Multi Message Format:
//   An OSC bundle with multiple messages, the end of each 
//   address being a key, and only one argument, 
//   the corresponding value:
// 		Bundle Size: 3
// 		Address 1: /LOOM/Device7/abc
// 		Value 1: 1
// 		Address 2: /LOOM/Device7/def
// 		Value 1: 2.34
// 		Address 3: /LOOM/Device7/ghi
// 		Value 1: "five"
//
// @param bndl     The OSC bundle to be converted
// @param outBndl  Where to store the converted bundle
// @param format   Output bundle format (SINGLEMSG or MULTIMSG)
// 
void convert_bundle_structure(OSCBundle *bndl, OSCBundle *outBndl, BundleStructure format)
{
	if (bndl->size() < 1) {
		LOOM_DEBUG_Println("Bundle has no valid contents, cannot be converted");
	}
	
	if ((format == MULTIMSG) && (bndl->size() > 1)) {
		deep_copy_bundle(bndl, outBndl);
		return; 
	}

	outBndl->empty();

	// Single to Multi
	if (format == SINGLEMSG) {
		aux_convert_bundle_structure_to_single(bndl, outBndl);
	} 

	// Multi to Single
	if (format == MULTIMSG) {
		aux_convert_bundle_structure_to_multi(bndl, outBndl);
	}

}

void convert_bundle_structure(OSCBundle *bndl, BundleStructure format)
{
	OSCBundle outBndl;
	convert_bundle_structure(bndl, &outBndl, format);
	deep_copy_bundle(&outBndl, bndl);
}





// ================================================================
// ===         CONVERSION FROM BUNDLE TO ARRAY FORMATS          ===
// ================================================================



// --- CONVERT OSC TO ARRAY KEY VALUE --- 
//
// Converts an OSC bundle to an array formated as:
// 	 [key1, value1, key2, value2, key3, value3]
// Converts bundle to flat single message if not already
//
// @param bndl        The bundle to use to fill the key value array
// @param key_values  The array to be populated
//
void convert_bundle_to_array_key_value(OSCBundle *bndl, String key_values[], int kv_len)
{	
	// Convert bundle to flat single message if not already
	OSCBundle convertedBndl;	

	if (bndl->size() > 1)  {
		convert_bundle_structure(bndl, &convertedBndl, SINGLEMSG); 
	} else {
		convertedBndl = *bndl;
	} 

	// Make sure key_values array is large enough
	if ( convertedBndl.getOSCMessage(0)->size() > kv_len ) {
		// LOOM_DEBUG_Println("Key-values array not large enough to hold all of bundle data, cannot convert");
		return;
	}

	// Fill key-value array
	OSCMessage* msg = convertedBndl.getOSCMessage(0);	
	for (int i = 0; i < msg->size(); i++) {
		key_values[i] = get_data_value(msg, i);
	}
}



// --- CONVERT OSC TO ARRAYS ASSOC --- 
//
// Converts an OSC bundle to two associated arrays,
// formated as:
//   [key1,   key2,   key3]
//   [value1, value2, value3]
// Converts bundle to flat single message if not already
//
// @param bndl   The bundle to use to fill the arrays
// @param keys   The array of keys be populated
// @param value  The array of values to be populated
//
void convert_bundle_to_arrays_assoc(OSCBundle *bndl, String keys[], String values[], int assoc_len)
{
	// Convert bundle to flat single message if not already
	OSCBundle convertedBndl;	
	if (bndl->size() > 1) {
		convert_bundle_structure(bndl, &convertedBndl, SINGLEMSG);
	} else {
		convertedBndl = *bndl;
	}

	// Make sure keys and values arrays are large enough
	if ( convertedBndl.getOSCMessage(0)->size() > 2*assoc_len ) {
		LOOM_DEBUG_Println("Key-values array not large enough to hold all of bundle data, cannot convert");
		return;
	}

	// Fill key and value arrays
	OSCMessage* msg = convertedBndl.getOSCMessage(0);
	for (int i = 0; i < msg->size(); i+=2) {
		keys[i/2]   = get_data_value(msg, i);
		values[i/2] = get_data_value(msg, i+1); 
	}
}


template <typename T>
void convert_bundle_to_array(OSCBundle *bndl, T data [], int len)
{
	String tmp_strings[len];
	convert_bundle_to_array_key_value(bndl, tmp_strings, len);
	convert_array(tmp_strings, data, len);
}


// Not verified to be fully functional yet
void convert_bundle_to_array_w_header(OSCBundle *bndl, String data [], int len)
{
	// LOOM_DEBUG_Println("In convert bundle to array w header");
	String tmpStrs[len-1];
	char buf[50];

	OSCMessage *msg;
	
	msg = bndl->getOSCMessage(0);
	msg->getAddress(buf, 0);
	// Serial.print("Address ");
	// Serial.println(buf);

	// print_bundle(bndl);
	// bndl->getOSCMessage(0)->getAddress(tmp);

	convert_bundle_to_array(bndl, tmpStrs, len-1);

	LOOM_DEBUG_Println("Done convert_bundle_to_array");

	// print_bundle(bndl);



	LOOM_DEBUG_Println("Done getting address");

	data[0] = String(buf);

	LOOM_DEBUG_Println("Done setting data 0");

	for (int i = 1; i < len; i++) {
		data[i] = tmpStrs[i-1];
	}

	LOOM_DEBUG_Println("Done copying");
}







// ================================================================
// ===             CONVERSION BETWEEN ARRAY FORMATS             ===
// ================================================================



// --- CONVERT ARRAY KEY VALUES TO ASSOC ---
// 
// Converts and array formatted as:
//   [key1, value1, key2, value2, key3, value3]
// to two associated arrays formatted as:
//   [key1,   key2,   key3]
//   [value1, value2, value3]
// 
// @param key_values  The flat array of keys and values
// @param keys        The array to be filled with the keys from 'key_values'
// @param values      The array to be filled with the values for 'key_values'
// @param kv_len      The length of the 'key_value' array
// @param assoc_len   The length of the 'keys' and 'values' arrays (should be the same)
//
void convert_array_key_value_to_assoc(String key_values [], String keys [], String values [], int kv_len, int assoc_len)
{
	if (kv_len > 2*assoc_len) {
		LOOM_DEBUG_Println("Keys or values array not large enough to hold all of keys/values, cannot split");
		return;
	}
	for (int i = 0; i < kv_len; i+=2) {
		keys[i/2]   = key_values[i];
		values[i/2] = key_values[i+1]; 	
	}
}



// --- CONVERT ARRAY KEY VALUES TO ASSOC ---
// 
// Converts two associated arrays formatted as:
//   [key1,   key2,   key3]
//   [value1, value2, value3]:
// to single array formatted as:
//   [key1, value1, key2, value2, key3, value3]
// 
// @param keys        The array of keys  
// @param values      The array to values
// @param key_values  The flat array of keys and values to be filled by combining 'keys' and 'values'
// @param assoc_len   The length of the 'keys' and 'values' arrays (should be the same)
// @param kv_len      The length of the 'key_value' array
//
void convert_array_assoc_to_key_value(String keys [], String values [], String key_values [], int assoc_len, int kv_len)
{
	if ( kv_len < 2*assoc_len ) {
		LOOM_DEBUG_Println("Key-values array is not at least twice the size of keys and values arrays, cannot merge");
		return;
	}
	for (int i = 0; i < assoc_len; i++) {
		key_values[i*2]   = keys[i];
		key_values[i*2+1] = values[i];
	}
}

template <typename T>
void convert_array_assoc_to_key_value(String keys [], T values [], String key_values [], int assoc_len, int kv_len)
{
	if ( kv_len < 2*assoc_len ) {
		LOOM_DEBUG_Println("Key-values array is not at least twice the size of keys and values arrays, cannot merge");
		return;
	}
	for (int i = 0; i < assoc_len; i++) {
		key_values[i*2]   = keys[i];
		key_values[i*2+1] = String(values[i]);
	}
}






// ================================================================
// ===         CONVERSION FROM ARRAY TO BUNDLE FORMATS          ===
// ================================================================

// Interpret is an optional parameter in the following 4 array to 
// bundle functions (8 if counting overloaded versions)
//
// Floats may not work correctly with 32u4
// 
// The encoding is as follows:
//  0 - 3 assume elements are in [key1, value1, key2, value2...] format,
//        with even indexes being keys, and will be left as strings
//        and odd indexes being data, and will be interpreted as specified
//   0:  Smart   (int->int, float->float, other->string) 
//                 [This is the default if interpret is not specified]
//   1: Int     (non-ints will become 0)
//   2: Float   (non-floats will become 0)
//   3: String  (does no extra manipulation, leaves as strings)
//
//  4-6 do NOT assume key value pairs and will also interpret even indexes as specified,
//      use with caution
//      *Note: likely will be issues when making multi-message bundles with below option
//   4: Smart-All 
//   5: Int-All
//   6: Float-All


//	
// Convert an array formatted as:
//   [key1, value1, key2, value2 ...]
// to an OSC bundle with either single or multiple messages
// 
// @param keys_values    The flat array of keys and values  
// @param bndl           The flat array of keys and values to be filled by combining 'keys' and 'values'
// @param packet_header  The address to add to bundle/messages 
// @param kv_len         The length of the 'keys_values' array 
// @param interprt       (see comment at start of section above for details about the parameter)
//
void convert_key_value_array_to_bundle(String key_values [], OSCBundle *bndl, char packet_header[], int kv_len, BundleStructure format, int interpret)
{
	if ((interpret < 0) || (interpret > 6)) {
		LOOM_DEBUG_Println3("'", interpret, "' is not a valid way to interpret array when converting to bundle");
		LOOM_DEBUG_Println("Use: 0=Smart, 1=Int, 2=Float, 3=String, 4=Smart-All, 5=Int-All, 6=Float-All");
		LOOM_DEBUG_Println("Omitting 'interpret' argument will default to 'Smart' (recommended)");
		return;
	}
	bndl->empty();

	OSCMessage tmpMsg;
	const char *number;    
	char    *end;       char  data[50];
	int32_t tmpInt;     float tmpFloat;

	// Convert array to single message bundle format
	for (int i = 0; i < kv_len; i++) {
		key_values[i].toCharArray(data, 50);

		// If all are string, or assuming keys as strings, add data as a string
		if ( (interpret == 3) || ((interpret <= 3) && (i%2==0)) ) {
			tmpMsg.add(data);
		} else {
			switch (interpret) {
				case 0: case 4: 	// Smart [All]
				case 1: case 5: 	// Int [All]
					tmpInt = (int32_t)strtol(data, &end, 10);
					if ( (interpret == 1) ||  (interpret == 5) || !(end == data || *end != '\0') ) {
						tmpMsg.add( tmpInt );   break;
					}
				case 2: case 6: 	// Float [All]
					#if is_m0 == 1
						tmpFloat = strtof(data, &end);
					#else
						tmpFloat = 0;
					#endif
					if ( (interpret == 2) || (interpret == 6) || !(end == data || *end != '\0') ) {
						tmpMsg.add( tmpFloat ); break;
					}
				default: 			// String
					tmpMsg.add(data);
			} // of switch
		} // of else
	} // of for

	// Add address string to message
	char address[80];
	if (packet_header[0] == '/') {
		sprintf(address, "%s/data", packet_header);
	} else {
		sprintf(address, "/%s/data", packet_header);
	}

	tmpMsg.setAddress(address);
	bndl->add(tmpMsg);

	// If multi-message format requested
	if (format == MULTIMSG) {
		convert_bundle_structure(bndl, MULTIMSG);
	}
}

void convert_key_value_array_to_bundle(String key_values [], OSCBundle *bndl, char packet_header[], int kv_len, BundleStructure format) {
	convert_key_value_array_to_bundle(key_values, bndl, packet_header, kv_len, format, 0); 
}



// --- CONVERT ASSOC ARRAYS TO BUNDLE ---
//
// Convert an array formatted as:
//   [key1, key2 ...]
//   [value1, value2 ...]
// to an OSC message with either single or multiple messages
//
// @param keys           The array of keys  
// @param values         The array to values
// @param bndl           The flat array of keys and values to be filled by combining 'keys' and 'values'
// @param packet_header  The address to add to bundle/messages 
// @param assoc_len      The length of the 'keys' and 'values' arrays (should be the same)
// @param format         The output bundle structure (single or multiple messages)
// @param interpret       (see comment at start of section above for details about the parameter)
//
void convert_assoc_arrays_to_bundle(String keys [], String values [], OSCBundle *bndl, char packet_header[], int assoc_len, BundleStructure format, int interpret)
{
	// Convert to single array first 
	int kv_len = 2*assoc_len;
	String key_values[kv_len];
	convert_array_assoc_to_key_value(keys, values, key_values, assoc_len, kv_len);
	// Then from key-value to desired bundle structure
	convert_key_value_array_to_bundle(key_values, bndl, packet_header, kv_len, format, interpret);
}

// OVERLOADED version of the previous function
// Simply calls the previous function with an 'interpret' value of 0
// (see comment at start of section above for details about the parameter)
// void convert_assoc_arrays_to_bundle(String keys [], String values [], OSCBundle *bndl, char packet_header[], int assoc_len, BundleStructure format) { 
// 	convert_assoc_arrays_to_bundle(keys, values, bndl, packet_header, assoc_len, format, 0); 
// }

template <typename T>
void convert_assoc_arrays_to_bundle(String keys [], T values [], OSCBundle *bndl, char packet_header[], int assoc_len, BundleStructure format)
{
	String converted_values[assoc_len];
	convert_array(values, converted_values, assoc_len);
	convert_assoc_arrays_to_bundle(keys, converted_values, bndl, packet_header, assoc_len, format, 0);

}

// Conversion from array of non-Strings to single message bundle   
template <typename T>   
void convert_array_to_bundle(T data [], OSCBundle *bndl, char packet_header[], int len)
{
	bndl->empty();
	bndl->add(packet_header);
	append_to_bundle(bndl, data, len);
}





// ================================================================
// ===              CONVERSION BETWEEN DATA TYPES               ===
// ================================================================


int convert_string_to_int(char * s) 
{ return (int)strtol(s, NULL, 10); }

float convert_string_to_float(char * s) 
{ return strtof(s, NULL); }

int convert_string_to_int(String s) 
{ return (int)strtol(s.c_str(), NULL, 10); }

float convert_string_to_float(String s) 
{ return strtof(s.c_str(), NULL); }




// ================================================================
// ===              CONVERSION BETWEEN ARRAY TYPES              ===
// ================================================================

void convert_array(String src [], int dest [], int count)
{ 
	char buf[20]; 
	for (int i = 0; i < count; i++) { 
		src[i].toCharArray(buf, 20); 
		dest[i] = (int)strtol(buf, NULL, 10); 
	} 
}

void convert_array(String src [], float dest [], int count)
{ 
	char buf[20]; 
	for (int i = 0; i < count; i++) { 
		src[i].toCharArray(buf, 20); 
		dest[i] = strtof(buf, NULL); 
	} 
}

void convert_array(String src [], char dest [][20], int count)
{ 
	for (int i = 0; i < count; i++) 
		src[i].toCharArray(dest[i], 10); 
}

template <typename Tin> 
void convert_array(Tin src[], String dest[], int count)
{ 
	for (int i = 0; i < count; i++) 
		dest[i] = String(src[i]); 
}

template <typename Tin, typename Tout> 
void convert_array(Tin src [], Tout dest[], int count)
{ 
	String tmps[count]; 
	convert_array(src, tmps, count); 
	convert_array(tmps, dest, count); 
}



// #endif // of #ifndef LOOM_TRANSLATOR_h

