
#include "Loom_Translator.h"
#include "Loom_Macros.h"


// ================================================================
// ===                      FLATTEN BUNDLE                      ===
// ================================================================


/////////////////////////////////////////////////////////////////////
void flatten_bundle(OSCBundle& bndl, OSCBundle& out_bndl)
{
	// Make sure bundle has more than one message
	// Check upper bound as well, as querying some empty bundles gives large value
	if ( (bndl.size() < 1) || (bndl.size() > 1000) ) {
		Println("Bundle has no valid contents, cannot be converted");
		return;
	}

	char address[50], suffix[50], key[30], buf[80];

	// Get device identification from address
	osc_extract_header_to_section(bndl.getOSCMessage(0), 5, address);
	sprintf(address, "%s%s", address, "/data");
	OSCMessage new_msg = OSCMessage(address);
	OSCMessage* msg;     // Temporarily hold message of bundle
	
	// Copy data of messages into new bundle
	for (int i = 0; i < bndl.size(); i++) {

		msg = bndl.getOSCMessage(i); 	// Get ith messsage
		osc_extract_header_from_section(msg, 7, suffix);

		for (int j = 0; j < msg->size(); j =j+2) {

			// Get Key
			switch (msg->getType(j)) {
				case 'f' : sprintf(key, "%f", msg->getFloat(j) );	break;
				case 'i' : sprintf(key, "%d", msg->getInt(j) ); 	break;
				case 's' : msg->getString(j, key, 20); 				break;
				default  : strcpy(key, "key");
			}

			// Possibly add suffix
			if (strlen(suffix) > 0) {
				sprintf(key, "%s%s", key, suffix);
			}

			new_msg.add(key);

			// Get Data Point
			switch (msg->getType(j+1)) {
				case 'f' : new_msg.add(msg->getFloat(j+1));	break;
				case 'i' : new_msg.add(msg->getInt(j+1));	break;
				case 's' : msg->getString(j+1, buf, 80);  new_msg.add(buf);  break;
				default  : Println("Unsupported data data_type.");
			}
		} // of for j
	} // of for i

	out_bndl.add(new_msg);
}

/////////////////////////////////////////////////////////////////////
void flatten_bundle(OSCBundle& bndl)
{
	OSCBundle out_bndl;
	flatten_bundle(bndl, out_bndl);
	deep_copy_bundle(out_bndl, bndl);
}








// ================================================================
// ===         CONVERSION FROM BUNDLE TO ARRAY FORMATS          ===
// ================================================================


/////////////////////////////////////////////////////////////////////
// --- CONVERT OSC TO ARRAY KEY VALUE --- 
//
// Converts an OSC bundle to an array formated as:
// 	 [key1, value1, key2, value2, key3, value3]
// Converts bundle to flat single message if not already
//
// @param bndl        The bundle to use to fill the key value array
// @param key_values  The array to be populated
//
void convert_bundle_to_array_key_value(OSCBundle& bndl, String key_values[], int kv_len)
{	
	OSCBundle converted_bndl;	
	flatten_bundle(bndl, converted_bndl);

	// Make sure key_values array is large enough
	if ( converted_bndl.getOSCMessage(0)->size() > kv_len ) {
		Println("Key-values array not large enough to hold all of bundle data, cannot convert");
		return;
	}

	// Fill key-value array
	OSCMessage* msg = converted_bndl.getOSCMessage(0);	
	for (int i = 0; i < msg->size(); i++) {
		key_values[i] = get_data_value(msg, i);
	}
}


/////////////////////////////////////////////////////////////////////
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
void convert_bundle_to_arrays_assoc(OSCBundle& bndl, String keys[], String values[], int assoc_len)
{
	OSCBundle converted_bndl;	
	flatten_bundle(bndl, converted_bndl);

	// Make sure keys and values arrays are large enough
	if ( converted_bndl.getOSCMessage(0)->size() > 2*assoc_len ) {
		Println("Key-values array not large enough to hold all of bundle data, cannot convert");
		return;
	}

	// Fill key and value arrays
	OSCMessage* msg = converted_bndl.getOSCMessage(0);
	for (int i = 0; i < msg->size(); i+=2) {
		keys[i/2]   = get_data_value(msg, i);
		values[i/2] = get_data_value(msg, i+1); 
	}
}







// ================================================================
// ===             CONVERSION BETWEEN ARRAY FORMATS             ===
// ================================================================


/////////////////////////////////////////////////////////////////////
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
		Println("Keys or values array not large enough to hold all of keys/values, cannot split");
		return;
	}

	for (int i = 0; i < kv_len; i+=2) {
		keys[i/2]   = key_values[i];
		values[i/2] = key_values[i+1]; 	
	}
}


/////////////////////////////////////////////////////////////////////
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
		Println("Key-values array is not at least twice the size of keys and values arrays, cannot merge");
		return;
	}

	for (int i = 0; i < assoc_len; i++) {
		key_values[i*2]   = keys[i];
		key_values[i*2+1] = values[i];
	}
}






// ================================================================
// ===         CONVERSION FROM ARRAY TO BUNDLE FORMATS          ===
// ================================================================

// Interpret is an optional parameter in the following 4 array to 
// bundle functions (8 if counting overloaded versions)
// 
// The encoding is as follows:
//  0 - 3 assume elements are in [key1, value1, key2, value2...] format,
//        with even indexes being keys, and will be left as strings
//        and odd indexes being data, and will be interpreted as specified
//    0:  Smart   (int->int, float->float, other->string) 
//                 [This is the default if interpret is not specified]
//    1: Int     (non-ints will become 0)
//    2: Float   (non-floats will become 0)
//    3: String  (does no extra manipulation, leaves as strings)
//
//  4-6 do NOT assume key value pairs and will also interpret even indexes as specified,
//      use with caution
//    4: Smart-All 
//    5: Int-All
//    6: Float-All

/////////////////////////////////////////////////////////////////////
// Convert an array formatted as:
//   [key1, value1, key2, value2 ...]
// to an OSC bundle with either single or multiple messages
// 
// @param keys_values    The flat array of keys and values  
// @param bndl           The flat array of keys and values to be filled by combining 'keys' and 'values'
// @param address  The address to add to bundle/messages 
// @param kv_len         The length of the 'keys_values' array 
// @param interprt       (see comment at start of section above for details about the parameter)
//
void convert_key_value_array_to_bundle(String key_values [], OSCBundle& bndl, char* address, int kv_len, int interpret)
{
	if ((interpret < 0) || (interpret > 6)) {
		Println3("'", interpret, "' is not a valid way to interpret array when converting to bundle");
		Println("Use: 0=Smart, 1=Int, 2=Float, 3=String, 4=Smart-All, 5=Int-All, 6=Float-All");
		Println("Omitting 'interpret' argument will default to 'Smart' (recommended)");
		return;
	}
	bndl.empty();

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
					tmpFloat = strtof(data, &end);
					if ( (interpret == 2) || (interpret == 6) || !(end == data || *end != '\0') ) {
						tmpMsg.add( tmpFloat ); break;
					}
				default: 			// String
					tmpMsg.add(data);
			} // of switch
		} // of else
	} // of for

	// Add address string to message
	char full_address[80];
	if (address[0] == '/') {
		sprintf(full_address, "%s/data", address);
	} else {
		sprintf(full_address, "/%s/data", address);
	}

	tmpMsg.setAddress(full_address);
	bndl.add(tmpMsg);
}



/////////////////////////////////////////////////////////////////////
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
// @param address  The address to add to bundle/messages 
// @param assoc_len      The length of the 'keys' and 'values' arrays (should be the same)
// @param format         The output bundle structure (single or multiple messages)
// @param interpret       (see comment at start of section above for details about the parameter)
//
void convert_assoc_arrays_to_bundle(String keys [], String values [], OSCBundle& bndl, char* address, int assoc_len, int interpret)
{
	// Convert to single array first 
	int kv_len = 2*assoc_len;
	String key_values[kv_len];
	convert_array_assoc_to_key_value(keys, values, key_values, assoc_len, kv_len);
	// Then from key-value to desired bundle structure
	convert_key_value_array_to_bundle(key_values, bndl, address, kv_len, interpret);
}




// ================================================================
// ===              CONVERSION BETWEEN DATA TYPES               ===
// ================================================================

/////////////////////////////////////////////////////////////////////
int convert_string_to_int(char* s) 
{ 
	return (int)strtol(s, NULL, 10); 
}

/////////////////////////////////////////////////////////////////////
float convert_string_to_float(char* s) 
{ 
	return strtof(s, NULL); 
}

/////////////////////////////////////////////////////////////////////
int convert_string_to_int(String s) 
{ 
	return (int)strtol(s.c_str(), NULL, 10); 
}

/////////////////////////////////////////////////////////////////////
float convert_string_to_float(String s) 
{ 
	return strtof(s.c_str(), NULL); 
}




// ================================================================
// ===              CONVERSION BETWEEN ARRAY TYPES              ===
// ================================================================

/////////////////////////////////////////////////////////////////////
void convert_array(String src [], int dest [], int count)
{ 
	char buf[20]; 
	for (int i = 0; i < count; i++) { 
		src[i].toCharArray(buf, 20); 
		dest[i] = (int)strtol(buf, NULL, 10); 
	} 
}

/////////////////////////////////////////////////////////////////////
void convert_array(String src [], float dest [], int count)
{ 
	char buf[20]; 
	for (int i = 0; i < count; i++) { 
		src[i].toCharArray(buf, 20); 
		dest[i] = strtof(buf, NULL); 
	} 
}

/////////////////////////////////////////////////////////////////////
void convert_array(String src [], char dest [][20], int count)
{ 
	for (int i = 0; i < count; i++) 
		src[i].toCharArray(dest[i], 10); 
}




