// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <string.h>


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
enum BundleStructure {
	SINGLEMSG,
	MULTIMSG
};

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
union data_value { // Used in translation between OSC and strings
	int32_t i;
	float f;
	uint32_t u;
};


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================

// Printing and data extraction functions
void   print_message(OSCMessage* msg);
void   print_message(OSCMessage* msg, bool detail);
void   print_bundle(OSCBundle *bndl);
template<typename T> 
void   print_array(T data [], int len, int format);
int    get_bundle_bytes(OSCBundle *bndl); 					// relatively untested
bool   bundle_empty(OSCBundle *bndl);
String get_data_value(OSCMessage* msg, int pos);
String get_address_string(OSCMessage *msg);
void   osc_extract_header_section(OSCMessage* msg, int section, char* result);
void   osc_extract_header_to_section(OSCMessage* msg, int section, char* result);
void   osc_extract_header_from_section(OSCMessage* msg, int section, char* result);

// Deep copy functions
void   deep_copy_bundle(OSCBundle *srcBndl, OSCBundle *destBndl);
void   deep_copy_message(OSCMessage *scrMsg, OSCMessage *destMsg);

// Conversions between bundles and strings (and auxiliary functions)
void convert_OSC_string_to_bundle(char *osc_string, OSCBundle*bndl);
void convert_OSC_bundle_to_string(OSCBundle *bndl, char *osc_string);
void original_convert_OSC_string_to_bundle(char *osc_string, OSCBundle*bndl);
void original_convert_OSC_bundle_to_string(OSCBundle *bndl, char *osc_string);
void uncompress_OSC_string(char *osc_string);
void str_replace(char *target, const char *needle, const char *replacement);
const char* nth_strchr(const char* s, char c, int n);

// Conversion between bundle formats
void convert_bundle_structure(OSCBundle *bndl, OSCBundle *outBndl, BundleStructure format);
void convert_bundle_structure(OSCBundle *bndl, BundleStructure format);


// Conversion from bundle to array formats
void convert_bundle_to_array_key_value(OSCBundle *bndl, String key_values[], int kv_len);
void convert_bundle_to_arrays_assoc(OSCBundle *bndl, String keys[], String values[], int assoc_len);

template <typename T>
void convert_bundle_to_array(OSCBundle *bndl, T data [], int len);
void convert_bundle_to_array_w_header(OSCBundle *bndl, String data [], int len);


// Conversion from array to bundle formats
void convert_key_value_array_to_bundle(String key_values [], OSCBundle *bndl, char packet_header[], int kv_len, BundleStructure format, int interpret);
void convert_key_value_array_to_bundle(String key_values [], OSCBundle *bndl, char packet_header[], int kv_len, BundleStructure format);

void convert_assoc_arrays_to_bundle(String keys [], String values [], OSCBundle *bndl, char packet_header[], int assoc_len, BundleStructure format, int interpret);
template <typename T>
void convert_assoc_arrays_to_bundle(String keys [], T values [], OSCBundle *bndl, char packet_header[], int assoc_len, BundleStructure format);

template <typename T>
void convert_array_to_bundle(T data [], OSCBundle *bndl, char packet_header[], int len);


// Conversion between array formats
void convert_array_key_value_to_assoc(String key_values [], String keys [], String values [], int kv_len, int assoc_len);
void convert_array_assoc_to_key_value(String keys [], String values [], String key_values [], int assoc_len, int kv_len);
template <typename T>
void convert_array_assoc_to_key_value(String keys [], T values [], String key_values [], int assoc_len, int kv_len);

// Conversion between array element types
void convert_array(String src [], int    dest [],   int count);
void convert_array(String src [], float  dest [],   int count);
void convert_array(String src [], char dest [][20], int count);
template <typename Tin> 
void convert_array(Tin src[], String dest[], int count);
template <typename Tin, typename Tout> 
void convert_array(Tin src [], Tout dest[], int count);


// Appending single element or array to single-message format bundles
void append_to_bundle(OSCBundle *bndl, int    elem);
void append_to_bundle(OSCBundle *bndl, String elem);
template <typename T> 
void append_to_bundle(OSCBundle *bndl, T elem);
template <typename T> 
void append_to_bundle(OSCBundle *bndl, T elements [], int count);



// ================================================================
// ===                    GENERAL FUNCTIONS                     ===
// ================================================================

// --- PRINT MESSAGE ---
//
// Prints an OSC Message
// 
// @param msg     The message to be printed
// @param detail  The formatting (0 for 1 line, 1 for detailed mulitple lines)
void print_message(OSCMessage* msg, bool detail) 
{
	#if LOOM_DEBUG == 1
		char buf[50];

		if (!detail) {
			LOOM_DEBUG_Print(get_address_string(msg).c_str());
			for (int i = 0; i < msg->size(); i++) {
				LOOM_DEBUG_Print("  ");
				switch(msg->getType(i)) {
					case 'f': LOOM_DEBUG_Print(msg->getFloat(i) ); break;
					case 'i': LOOM_DEBUG_Print(msg->getInt(i)   ); break;
					case 's': msg->getString(i, buf, 50); LOOM_DEBUG_Print(buf); break;
					default: break;
				}
			}
			LOOM_DEBUG_Println();
		} else {
			LOOM_DEBUG_Println2("Address: ", get_address_string(msg).c_str());
			for (int i = 0; i < msg->size(); i++) {
				LOOM_DEBUG_Print3("Value (", i, ") ");
				switch(msg->getType(i)) {
					case 'f': LOOM_DEBUG_Println2("(f) ", msg->getFloat(i) ); break;
					case 'i': LOOM_DEBUG_Println2("(i) ", msg->getInt(i) );   break;
					case 's': msg->getString(i, buf, 50); LOOM_DEBUG_Println2("(s) ", buf ); break;
					default: break;
				}
			}
		}
	#endif // of 'LOOM_DEBUG == 1'
}

void print_message(OSCMessage* msg) 
{
	print_message(msg, 1);
}



// --- PRINT BUNDLE ---
//
// ** NOTE: Can make bundles unreadable after printing, use with caution **
//
// Print out the contents and structure of an OSC bundle
//
// @param bndl  An OSC bundle to be printed
//
void print_bundle(OSCBundle *bndl)
{
	if (bundle_empty(bndl)) return;

	#if LOOM_DEBUG == 1
		char buf[50];
		char data_type;
		LOOM_DEBUG_Println2("\nBundle Size: ", bndl->size());
		OSCMessage *msg;

		for (int i = 0; i < bndl->size(); i++) {
			LOOM_DEBUG_Println2("Message: ", i);
			print_message(bndl->getOSCMessage(i));
		}
		Serial.println();
	#endif
}


// 3 Options
//   1: every element on different line
//   2: every element on same line
//   3: 5 elements per line
template<typename T> 
void print_array(T data [], int len, int format)
{
	#if LOOM_DEBUG == 1
		for (int i = 0; i < len; i++) {
			if (format == 1) { Serial.println(data[i]); }
			if (format  > 1) { Serial.print(data[i]); Serial.print(" "); }
			if ((format > 2) && (i%5==0)) { Serial.println(); }
		}
		Serial.println();
	#endif
}




// --- GET BUNDLE BYTES ---
//
// Gets the size of a bundle in bytes
//
// @param bndl  The bndl to get the size of
// 
// @return The size of the bundle in bytes
// 
int get_bundle_bytes(OSCBundle *bndl)
{
	int total = 0;
	for (int i = 0; i < bndl->size(); i++) {
		total += bndl->getOSCMessage(i)->bytes();
	}
}


bool bundle_empty(OSCBundle *bndl)
{
	return get_bundle_bytes(bndl) == 0;
}



// --- GET DATA VALUE ---
// 
// @param msg  An OSCMessage, 
// @param pos  The position of an argument inside the OSCMessage.
//
// @return The value of the argument as a string.
//
String get_data_value(OSCMessage* msg, int pos) 
{
	if (pos < msg->size()) {
		switch (msg->getType(pos)) {
			case 'i':
				return String(msg->getInt(pos));
				break;
			case 'f':
				return String(msg->getFloat(pos));
				break;
			case 's':
				char buf[256];
				msg->getString(pos, buf, 256);
				return String(buf);
				break;
			default:
				LOOM_DEBUG_Println("Unsupported data data_type.");
				return String("");
		}
	}
	LOOM_DEBUG_Println2("Message does not have an argument at position: ", pos);

	return String("");
}


// --- GET ADDRESS STRING ---
//
// Returns a string object of the provided
// message's address string
//
String get_address_string(OSCMessage *msg)
{
	char buf[50];
	msg->getAddress(buf, 0);
	return String(buf);
}



// --- OSC EXTRACT HEADER SECTION ---
//
// Select a single part of an OSC Message header
// Sections are separated by '/'s
// Result does not include '/'s
//
// @param msg      The message to parse the header of 
// @param section  Which section to extract (1 indexed)
// @param result   Pointer to the char array to be filled
//
void osc_extract_header_section(OSCMessage* msg, int section, char* result)
{
	msg->getAddress(result);
	const char* cPtr_start = nth_strchr(result, '/', section);
	const char* cPtr_end   = nth_strchr(result, '/', section+1);
	if (cPtr_end == NULL) {
		cPtr_end = result + strlen(result);
	}
	snprintf(result, cPtr_end-cPtr_start, "%s\0", cPtr_start+1); 
}

// --- OSC EXTRACT HEADER TO SECTION ---
//
// Select up to and including the specified section of an osc message
// 
// @param msg      The message to parse the header of 
// @param section  Which section to extract up to (inclusive) (1 indexed)
// @param result   Pointer to the char array to be filled
//
void osc_extract_header_to_section(OSCMessage* msg, int section, char* result)
{
	msg->getAddress(result);
	const char* cPtr_end   = nth_strchr(result, '/', section+1);
	if (cPtr_end == NULL) {
		cPtr_end = result + strlen(result);
	}
	snprintf(result, cPtr_end-result+1, "%s\0", result); 
}

// --- OSC EXTRACT HEADER FROM SECTION ---  
//
// Select from a section of an OSC message to the end
//
// @param msg      The message to parse the header of 
// @param section  Which section to start extracting from (inclusive) (1 indexed)
// @param result   Pointer to the char array to be filled
//
void osc_extract_header_from_section(OSCMessage* msg, int section, char* result)
{
	msg->getAddress(result);
	const char* cPtr_start = nth_strchr(result, '/', section);
	if (cPtr_start == NULL) {
		sprintf(result, "\0"); 
	} else {
		sprintf(result, "%s\0", cPtr_start); 
	}
}



// --- DEEP COPY BUNDLE ---
// 
// Takes two bundle pointers,
// Copies the data of the first into the second
// 
// @param srcBndl   The source bundle to be copied
// @param destBndl  The bundle to copied into
//
void deep_copy_bundle(OSCBundle *srcBndl, OSCBundle *destBndl) 
{
	destBndl->empty();
	OSCMessage *msg;
	OSCMessage tmpMsg;
	char buf[50];
	for (int i = 0; i < srcBndl->size(); i++) { 	// for each message
		msg = srcBndl->getOSCMessage(i);
		for (int j = 0; j < msg->size(); j++) { 	// for each argument 

			switch (msg->getType(j)) {
	 			case 'i': tmpMsg.add(msg->getInt(j));	break;
	 			case 'f': tmpMsg.add(msg->getFloat(j));	break;
	 			case 's': char buf[80];  msg->getString(j, buf, 80);  tmpMsg.add(buf);  break;
	 			default: LOOM_DEBUG_Println("Unsupported data data_type.");
	 		}
 		}
 		msg->getAddress(buf);
		tmpMsg.setAddress(buf);

		destBndl->add(tmpMsg);
		tmpMsg.empty();		
	} 
}

void deep_copy_message(OSCMessage *srcMsg, OSCMessage *destMsg)
{
	LOOM_DEBUG_Println();
	char buf[50];
	for (int i = 0; i < srcMsg->size(); i++) {
		switch (srcMsg->getType(i)) {
 			case 'i': destMsg->add(srcMsg->getInt(i));	    break;
 			case 'f': destMsg->add(srcMsg->getFloat(i));	break;
 			case 's': char buf[80];  srcMsg->getString(i, buf, 80);  destMsg->add(buf);  break;
 			default: LOOM_DEBUG_Println("Unsupported data data_type.");
 		}
	}
	srcMsg->getAddress(buf);
	destMsg->setAddress(buf);
}


// ================================================================
// ===         CONVERSIONS BETWEEN BUNDLES AND STRINGS          ===
// ================================================================






// --- CONVERT OSC TO STRING ---
// 
// Converts an OSC Bundle to equivalent string to be used in LoRa transmissions
// Osc_string's contents will now include the OSCBundle's formatted data.
//
// @param bndl        An OSCBundle to put into string format.
// @param osc_string  A char * to fill with the OSCBundle's data.
//
void convert_OSC_bundle_to_string(OSCBundle *bndl, char *osc_string) 
{
	// This is done in case the bundle converts to a string larger than
	// 251 characters before compression
	char larger_buf[384];
	memset(larger_buf, '\0', sizeof(larger_buf));
	original_convert_OSC_bundle_to_string(bndl, (char*)larger_buf);
	// strcpy(osc_string, (const char*)larger_buf);
	// original_convert_OSC_bundle_to_string(bndl, osc_string);

	// LOOM_DEBUG_Println4("Larger Buf – [Len: ", strlen(larger_buf), "]\n", larger_buf);


	// LOOM_DEBUG_Println4("Before Compression – [Len: ", strlen(osc_string), "]\n", osc_string);

	const char* cPtr = nth_strchr(larger_buf, '/', 3);
	char buf[30];
	snprintf(buf, cPtr-larger_buf+2, "%s\0", larger_buf); // Copy compressable header to buf
	str_replace((char*)cPtr, buf, "%");

	snprintf(osc_string, 250, "%s\0", larger_buf);

	// LOOM_DEBUG_Print4("Last Char-1: [", strlen(osc_string), "] ", osc_string[strlen(osc_string)-2]); 
	// LOOM_DEBUG_Print(" (");
	// Serial.print(osc_string[strlen(osc_string)-2], HEX);
	// LOOM_DEBUG_Println(")");

	// LOOM_DEBUG_Print4("Last Char: [", strlen(osc_string), "] ", osc_string[strlen(osc_string)-1]); 
	// LOOM_DEBUG_Print(" (");
	// Serial.print(osc_string[strlen(osc_string)-1], HEX);
	// LOOM_DEBUG_Println(")");

	// LOOM_DEBUG_Print4("Last Char+1: [", strlen(osc_string), "] ", osc_string[strlen(osc_string)]); 
	// LOOM_DEBUG_Print(" (");
	// Serial.print(osc_string[strlen(osc_string)], HEX);
	// LOOM_DEBUG_Println(")");

	// Remove occasional trailing space
	if (osc_string[strlen(osc_string)-1] == 32 ) {
		osc_string[strlen(osc_string)-1] = '\0';
	}

	// LOOM_DEBUG_Print4("Last Char-1: [", strlen(osc_string), "] ", osc_string[strlen(osc_string)-2]); 
	// LOOM_DEBUG_Print(" (");
	// Serial.print(osc_string[strlen(osc_string)-2], HEX);
	// LOOM_DEBUG_Println(")");

	// LOOM_DEBUG_Print4("Last Char: [", strlen(osc_string), "] ", osc_string[strlen(osc_string)-1]); 
	// LOOM_DEBUG_Print(" (");
	// Serial.print(osc_string[strlen(osc_string)-1], HEX);
	// LOOM_DEBUG_Println(")");

	// LOOM_DEBUG_Print4("Last Char+1: [", strlen(osc_string), "] ", osc_string[strlen(osc_string)]); 
	// LOOM_DEBUG_Print(" (");
	// Serial.print(osc_string[strlen(osc_string)], HEX);
	// LOOM_DEBUG_Println(")");

	// LOOM_DEBUG_Println4("After Compression – [Len: ", strlen(osc_string), "]\n", osc_string);
}



// --- CONVERT STRING TO OSC ---
// 
// Converts string (generally used by LoRa/nRF) to an equivalent OSCBundle 
// Added compression over original version
//
// @param osc_string  A char * created through the use of convert_OSC_bundle_to_string(), 
// @param bndl        The OSC bundle to be populated
//
void convert_OSC_string_to_bundle(char *osc_string, OSCBundle* bndl) 
{
	uncompress_OSC_string(osc_string); 
	original_convert_OSC_string_to_bundle(osc_string, bndl);
}



void uncompress_OSC_string(char* osc_string) 
{
	// LOOM_DEBUG_Println4("Before Restore – [Len: ", strlen(osc_string), "]\n", osc_string);


	// char* restored_str = new char[512];

	char buf[30];
	const char* cPtr = nth_strchr(osc_string, '/', 3);
	
	snprintf(buf, cPtr-osc_string+2, "%s", osc_string);
	// LOOM_DEBUG_Print3(" (", buf, ")");
	// LOOM_DEBUG_Println3(" + (", cPtr+1, ")");

	str_replace((char*)cPtr, "%", buf);	

	// LOOM_DEBUG_Println2("Len in function: ", strlen(osc_string));
	// LOOM_DEBUG_Println2("FIXED?\n", osc_string); // This one is fine
	// sprintf(osc_string_out, "%s\0", osc_string);

	// strcpy(restored_str, osc_string);
	// osc_string = restored_str;
	// return restored_str;
	// LOOM_DEBUG_Println2("\nRESTORE?\n", restored_str);
	// osc_string = restored_str;


	// LOOM_DEBUG_Println4("After Restore – [Len: ", strlen(osc_string), "]\n", osc_string);
}


// Finds nth instance of a character in a string
// Auxiliary function for OSC string compression
const char* nth_strchr(const char* s, char c, int n)
{
	int c_count;
	char* nth_ptr;

	for (c_count=1,nth_ptr=strchr(s,c);  nth_ptr != NULL && c_count < n && c!=0; c_count++) { 
		nth_ptr = strchr(nth_ptr+1, c); 
	}

	return nth_ptr;
}


// Replaces substrings with other substrings in a string
// Auxiliary function for OSC string compression
void str_replace(char *target, const char *needle, const char *replacement)
{
	char buffer[384] = { 0 };
	char *insert_point = &buffer[0];
	const char *tmp = target;
	size_t needle_len = strlen(needle);
	size_t repl_len = strlen(replacement);

	while (1) {
		const char *p = strstr(tmp, needle);

		// walked past last occurrence of needle; copy remaining part
		if (p == NULL) {
			strcpy(insert_point, tmp);
			break;
		}

		// copy part before needle
		memcpy(insert_point, tmp, p - tmp);
		insert_point += p - tmp;

		// copy replacement string
		memcpy(insert_point, replacement, repl_len);
		insert_point += repl_len;

		// adjust pointers, move on
		tmp = p + needle_len;
	}

    // write altered string back to target
	strcpy(target, buffer);
}


void original_convert_OSC_string_to_bundle(char *osc_string, OSCBundle* bndl) 
{
	bndl->empty();
	data_value value_union;
	char buf[strlen(osc_string)+1];
	char *p = buf, *p2 = NULL;
	char *token = NULL, *msg_token = NULL; 
	strcpy(buf, osc_string);
	OSCMessage *msg;
	msg_token = strtok_r(p, " ", &p);
	while (msg_token != NULL & strlen(msg_token) > 0) 
	{
		p2 = msg_token;
		token = strtok_r(p2, ",", &p2);
		msg = &(bndl->add(token));
		token = strtok_r(NULL, ",", &p2);
		int k = 1;
		while (token != NULL & strlen(token) > 0) 
		{
			if (token[0] == 'f') {
				value_union.u = strtoul(&token[1], NULL, 0);
				msg->add(value_union.f);
			}
			else if (token[0] == 'i') {
				value_union.u = strtoul(&token[1], NULL, 0);
				msg->add(value_union.i);
			}
			else if (token[0] == 's') {
				msg->add(&token[1]);
			}
			token = strtok_r(p2, ",", &p2);
		}
		msg_token = strtok_r(p, " ", &p);
	}
}

void original_convert_OSC_bundle_to_string(OSCBundle *bndl, char *osc_string) 
{
	char data_type;
	data_value value;
	int addr_len = 40;
	OSCMessage* msg; 
	char addr_buf[addr_len];

	memset(osc_string, '\0', sizeof(osc_string));

	for (int i = 0; i < bndl->size(); i++) {
		msg = bndl->getOSCMessage(i);
		memset(addr_buf, '\0', addr_len);
		msg->getAddress(addr_buf, 0);
		strcat(osc_string, addr_buf);

		for (int j = 0; j < msg->size(); j++) {
			data_type = msg->getType(j);
			switch (data_type) {
				case 'f':
					value.f = msg->getFloat(j);
					snprintf(addr_buf, addr_len, ",f%lu", value.u);
					strcat(osc_string, addr_buf);
					break;

				case 'i':
					value.i = msg->getInt(j);
					snprintf(addr_buf, addr_len, ",i%lu", value.u);
					strcat(osc_string, addr_buf);
					break;

				case 's':
					char data_buf[40];
					msg->getString(j, data_buf, sizeof(data_buf));
					snprintf(addr_buf, addr_len, ",s%s", data_buf);
					strcat(osc_string, addr_buf);
					break;

				default:
					if (data_type != '\0') {
						LOOM_DEBUG_Println("Invalid message arg type");
					}
					break;
			}
		}
		if (msg != NULL) strcat(osc_string, " ");
	}
}







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

		// Get ith messsage key and add to new message
		char tmpBuf[50], keyBuf[50];
		msg->getAddress(tmpBuf);
		strncpy(keyBuf, strrchr(tmpBuf,'/')+1, 49);
		newMsg.add(keyBuf);

		// Get ith message data and add to new message
		switch (msg->getType(0)) {
			case 'f': newMsg.add(msg->getFloat(0));	break;
			case 'i': newMsg.add(msg->getInt(0));	break;
			case 's': char buf[80];  msg->getString(0, buf, 50);  newMsg.add(buf);  break;
			default: LOOM_DEBUG_Println("Unsupported data data_type.");
		}
	}
	outBndl->add(newMsg);
}

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
			case 's': char buf[80];  msg->getString(i, buf, 80);  tmpMsg.add(buf);  break;
			default: LOOM_DEBUG_Println("Unsupported data data_type.");
		}

		// Add message as /address/key value
		snprintf(newAddress, strrchr(address,'/')-address+1, "%s", address);
		msg->getString(i-1, keyBuf, 80);
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
	
	// If already in requested message format
	if ((format == SINGLEMSG) && (bndl->size() == 1)) { 
		// LOOM_DEBUG_Println("Bundle already in single-message format");
		deep_copy_bundle(bndl, outBndl);
		return;
	}
	if ((format == MULTIMSG) && (bndl->size() > 1)) {
		// LOOM_DEBUG_Println("Bundle already in multi-message format");
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
	// LOOM_DEBUG_Println("In convert_bundle_to_array_key_value");


	// Convert bundle to flat single message if not already
	OSCBundle convertedBndl;	

	// LOOM_DEBUG_Println("Created convertedBndl");

	if (bndl->size() > 1)  {
		convert_bundle_structure(bndl, &convertedBndl, SINGLEMSG); 
	} else {
		convertedBndl = *bndl;
	} 

	// LOOM_DEBUG_Println("After bndl size check");

	// Make sure key_values array is large enough
	if ( convertedBndl.getOSCMessage(0)->size() > kv_len ) {
		// LOOM_DEBUG_Println("Key-values array not large enough to hold all of bundle data, cannot convert");
		return;
	}

	// Fill key-value array
	OSCMessage* msg = convertedBndl.getOSCMessage(0);	
	for (int i = 0; i < msg->size(); i++) {
		key_values[i] = get_data_value(msg, i);
		// key_values[i] = String(i); 
	}

	// LOOM_DEBUG_Println("Done with convert_bundle_to_array_key_value");

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
	// LOOM_DEBUG_Println("In convert bundle to array");

	String tmp_strings[len];

	// LOOM_DEBUG_Println("Created tmp_strings");

	convert_bundle_to_array_key_value(bndl, tmp_strings, len);

	// LOOM_DEBUG_Println("Before convert array");

	convert_array(tmp_strings, data, len);

	// LOOM_DEBUG_Println("Done with convert array");
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

		// If all are stringm, or assuming keys as strings, add data as a string
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

#if is_m0 == 1
void convert_array(String src [], float dest [], int count)
{ 
	char buf[20]; 
	for (int i = 0; i < count; i++) { 
		src[i].toCharArray(buf, 20); 
		dest[i] = strtof(buf, NULL); 
	} 
}
#endif

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


// ================================================================
// ===          APPENDING DATA TO EXISTING STRUCTURES           ===
// ================================================================

// OVERLOADED function for appending to a single-message format OSC bundle
void append_to_bundle(OSCBundle *bndl, int elem) 
{ 
	bndl->getOSCMessage(0)->add((int32_t)elem); 
}

void append_to_bundle(OSCBundle *bndl, String elem)
{ 
	bndl->getOSCMessage(0)->add(elem.c_str()); 
}

template <typename T> 
void append_to_bundle(OSCBundle *bndl, T elem)
{ 
	bndl->getOSCMessage(0)->add(elem);
}

template <typename T>
void append_to_bundle(OSCBundle *bndl, T elements [], int count)
{ 
	for (int i = 0; i < count; i++) 
		append_to_bundle(bndl, elements[i]); 
}









