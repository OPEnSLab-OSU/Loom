
#include "Loom_Misc.h"
#include "Loom_Macros.h"



String get_data_value(OSCMessage* msg, int pos) 
{
	if (pos < msg->size()) {
		switch (msg->getType(pos)) {
			case 'i':
				return String(msg->getInt(pos));
			case 'f':
				return String(msg->getFloat(pos));
			case 's':
				char buf[256];
				msg->getString(pos, buf, 256);
				return String(buf);
			default:
				LOOM_DEBUG_Println("Unsupported data data_type.");
				return String("");
		}
	}
	LOOM_DEBUG_Println2("Message does not have an argument at position: ", pos);

	return String("");
}


String get_address_string(OSCMessage *msg)
{
	char buf[50];
	msg->getAddress(buf, 0);
	return String(buf);
}





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
			LOOM_DEBUG_Println2("\tAddress: ", get_address_string(msg).c_str());
			for (int i = 0; i < msg->size(); i++) {
				LOOM_DEBUG_Print3("\tValue (", i, ") ");
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


void print_bundle(OSCBundle *bndl)
{
	if (!bndl->size()) return;

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

// Finds nth instance of a character in a string
// Auxiliary function for OSC string compression
const char* nth_strchr(const char* s, char c, int n)
{
	int c_count;
	char* nth_ptr;

	for (c_count=1,nth_ptr=strchr(s,c); nth_ptr != NULL && c_count < n && c!=0; c_count++) { 
		nth_ptr = strchr(nth_ptr+1, c); 
	}

	return nth_ptr;
}


// --- REPLACE CHARACTER ---
//
// Given a string, replace all instances of 'orig' char with 'rep' char
// Used primarily for replacing '~'s sent by Max
// as it cannot send strings with spaces
//
// @param str   Pointer to string to alter
// @param orig  Character to replace
// @param rep   Replacement character
// 
void replace_char(char *str, char orig, char rep) 
{
	char *ix = str;
	while((ix = strchr(ix, orig)) != NULL) {
		*ix++ = rep;
	}
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

// This version works with sprintf and comparisons like strcmp, but not printing
char* osc_extract_header_section(OSCMessage* msg, int section)
{
	char result[50];
	osc_extract_header_section(msg, section, result);
	return (char*)result;
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

// This version works with sprintf and comparisons like strcmp, but not printing
char* osc_extract_header_to_section(OSCMessage* msg, int section)
{
	char result[50];
	osc_extract_header_to_section(msg, section, result);
	return (char*)result;
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

// This version works with sprintf and comparisons like strcmp, but not printing
char* osc_extract_header_from_section(OSCMessage* msg, int section)
{
	char result[50];
	osc_extract_header_from_section(msg, section, result);
	return (char*)result;
}




// --- OSC EXTRACT NUMBER OF SECTIONS ---
//
// Return the number of sections in the OSC address of 
// an OSC message or bundle (first message)
// 
// @return Number of sections
//
int osc_address_section_count(String s)
{
	int count = 0;
	for (int i = 0; i < s.length(); i++) 
		if (s[i] == '/') count++;

	return count;
}

int osc_address_section_count(OSCMessage* msg)
{ 
	return osc_address_section_count(get_address_string(msg)); 
}

int osc_address_section_count(OSCBundle* bndl)
{ 
	return osc_address_section_count(bndl->getOSCMessage(0)); 
}







// ** VERSIONS FOR NEW ADDRESSING SCHEME **





// void osc_extract_family(OSCMessage* msg, char* result)
// {
// 	osc_extract_header_section(msg, 1, result);	
// 	snprintf(result, strchr(result, '_') - result + 1 , "%s", result);
// }

// char* osc_extract_family(OSCMessage* msg)
// {
// 	char result[50];
// 	osc_extract_family(msg, result);
// 	return (char*)result;
// }


// void osc_extract_family(OSCBundle* bndl, char* result)
// {
// 	if ((bndl) && (bndl->size()>0)) {
// 		osc_extract_family(bndl->getOSCMessage(0), result);
// 	} else {
// 		sprintf(result, "\0");
// 	}
// }

// char* osc_extract_family(OSCBundle* bndl)
// {
// 	if ((bndl) && (bndl->size()>0)) {
// 		return osc_extract_family(bndl->getOSCMessage(0));
// 	} else {
// 		return "";
// 	}
// }



// // --- OSC EXTRACT FAMILY NUMBER --- 
// //
// // Get the family number from a bundle or message
// // Assumes /<family>_# is first part of address
// // 
// // @return Family number
// //
// int osc_extract_family_number(OSCMessage* msg) 
// {
// 	if ( osc_address_section_count(msg) < 3 ) 
// 		return -1;
// 	char* ptr = strchr( osc_extract_header_section(msg, 1) , '_' );
// 	return (ptr) 
// 			? (int)strtol(ptr+1, NULL, 10) 
// 			: -1;
// }

// int osc_extract_family_number(OSCBundle* bndl) 
// {
// 	return ((bndl) && (bndl->size()>0)) 
// 			? osc_extract_family_number(bndl->getOSCMessage(0)) 
// 			: -1;
// 	// if ((bndl) && (bndl->size()>0)) {
// 	// 	return osc_extract_family_number(bndl->getOSCMessage(0)) 
// 	// } else {
// 	// 	return -1
// 	// }

// }




// add the char* return versions here



char get_message_type(OSCMessage* msg)
{
	char buf[50];
	msg->getAddress(buf, 0);
	return buf[1];
}


int extract_family_number(OSCMessage* msg)
{
	switch( get_message_type(msg) ) {
		case 'D': case 'S': 
			return (int)strtol( osc_extract_header_section(msg, 3) , NULL, 10);
		default : 
			return -1; 
	}
}

int extract_family_number(OSCBundle* bndl)
{ 
	return extract_family_number(bndl->getOSCMessage(0)); 
}



int extract_device_number(OSCMessage* msg)
{
	if ( get_message_type(msg) == 'D' ) {
		return (int)strtol( osc_extract_header_section(msg, 5) , NULL, 10);
	} else {
		return -1;
	}
}

int extract_device_number(OSCBundle* bndl)
{ 
	return extract_device_number(bndl->getOSCMessage(0)); 
}



void extract_family(OSCMessage* msg, char* result)
{ 
	osc_extract_header_section(msg, 2, result); 
}

void extract_family(OSCBundle* bndl, char* result)
{ 
	extract_family(bndl->getOSCMessage(0), result); 
}

char* extract_family(OSCMessage* msg)
{
	char result[50];
	extract_family(msg, result);
	return (char*)result;
}

char* extract_family(OSCBundle* bndl)
{ 
	return extract_family(bndl->getOSCMessage(0)); 
}




void extract_device(OSCMessage* msg, char* result)
{
	if ( get_message_type(msg) == 'D' ) {
		osc_extract_header_section(msg, 4, result);
	} else {
		sprintf(result, "\0");
	}
}

void extract_device(OSCBundle* bndl, char* result)
{ 
	extract_device(bndl->getOSCMessage(0), result); 
}

char* extract_device(OSCMessage* msg)
{
	char result[50];
	extract_device(msg, result);
	return (char*)result;
}

char* extract_device(OSCBundle* bndl)
{ 
	return extract_device(bndl->getOSCMessage(0)); 
}





















// void append_to_bundle_key_value_aux(OSCBundle *bndl, char* address_string, char* id_header, char* key)
// {
// 	char tmp[50];
// 	if ( bndl->size() ) {
// 		osc_extract_header_to_section(bndl->getOSCMessage(0), 2, tmp);
// 		sprintf(address_string, "%s/%s", tmp, key);
// 	} else {
// 		sprintf(address_string, "%s%d/%s%d%s", FAMILY, FAMILY_NUM, DEVICE, INIT_INST, key);
// 	}
// }




// --- APPEND TO BUNDLE KEY VALUE ---
//
// Append a key-value pair to a bundle
// Adds one message to the bundle, with an 
// address correlating to the key, and data 
// to the provided value
// //
// void append_to_bundle_key_value(OSCBundle *bndl, char* id_header, char* key, int elem)
// {
// 	char address_string[80];
// 	sprintf(address_string, "%s/%s", id_header, key);
// 	bndl->add(address_string).add( (int32_t)elem );
// }

// void append_to_bundle_key_value(OSCBundle *bndl, char* id_header, char* key, uint16_t elem)
// {
// 	char address_string[80];
// 	sprintf(address_string, "%s/%s", id_header, key);
// 	bndl->add(address_string).add( (int32_t)elem );
// }

// void append_to_bundle_key_value(OSCBundle *bndl, char* id_header, char* key, float elem)
// {
// 	char address_string[80];
// 	sprintf(address_string, "%s/%s", id_header, key);
// 	bndl->add(address_string).add( elem );
// }


// void append_to_bundle_key_value(OSCBundle *bndl, char* id_header, char* key, String elem)
// {
// 	char address_string[80];
// 	sprintf(address_string, "%s/%s", id_header, key);
// 	bndl->add(address_string).add( elem.c_str() );
// }




// template <typename T> 
// void append_to_bundle_key_value(OSCBundle *bndl, char* id_header, char* key, T elem)
// {
// 	char address_string[80];
// 	sprintf(address_string, "%s/%s", id_header, key);

// 	bndl->add(address_string).add( elem );
// }

// template void append_to_bundle_key_value<char*>(OSCBundle *bndl, char* id_header, char* key, char* elem);

// enum AppendType { NEW_MSG, LAST_MSG };





void append_to_bundle_aux(OSCBundle* bndl, char* key, int elem, int message_index)
{ bndl->getOSCMessage(message_index)->add(key).add( (int32_t)elem ); }

void append_to_bundle_aux(OSCBundle* bndl, char* key, uint16_t elem, int message_index)
{ bndl->getOSCMessage(message_index)->add(key).add( (int32_t)elem ); }

void append_to_bundle_aux(OSCBundle* bndl, char* key, float elem, int msg_idx)
{ bndl->getOSCMessage(msg_idx)->add(key).add( elem ); }

void append_to_bundle_aux(OSCBundle* bndl, char* key, String elem, int msg_idx)
{ bndl->getOSCMessage(msg_idx)->add(key).add( elem.c_str() ); }

void append_to_bundle_aux(OSCBundle* bndl, int key, int elem, int message_index)
{ bndl->getOSCMessage(message_index)->add( (int32_t)key ).add( (int32_t)elem ); }

void append_to_bundle_aux(OSCBundle* bndl, int key, uint16_t elem, int message_index)
{ bndl->getOSCMessage(message_index)->add( (int32_t)key ).add( (int32_t)elem ); }

void append_to_bundle_aux(OSCBundle* bndl, int key, float elem, int msg_idx)
{ bndl->getOSCMessage(msg_idx)->add( (int32_t)key ).add( elem ); }

void append_to_bundle_aux(OSCBundle* bndl, int key, String elem, int msg_idx)
{ bndl->getOSCMessage(msg_idx)->add( (int32_t)key ).add( elem.c_str() ); }














// template <typename T> 
// void append_to_bundle_msg_key_value_new_aux(OSCBundle* bndl, char* key, T elem, int msg_idx)
// {
// 	bndl->getOSCMessage(msg_idx)->add(key).add( elem );
// }


// int append_determine_index(OSCBundle* bndl, int msg_idx)
// {
// 	// Index valid
// 	if ( (msg_idx >= 0) && (msg_idx < bndl->size()) ) {
// 		append_to_bundle_msg_key_value_new_aux(bndl, key, elem, msg_idx);
// 	} 
// 	// Add to last message
// 	else if (msg_idx == -1) {
// 		append_to_bundle_msg_key_value_new_aux(bndl, key, elem, bndl->size()-1);
// 	}
// 	// Create new or invalid index
// 	else {
// 		bndl->add(id_header);
// 		append_to_bundle_msg_key_value_new_aux(bndl, key, elem, bndl->size()-1);
// 	}
// }


// // msg_idx of -1 means add to last message
// // msg_idx of -2 means add create new message
// // any other message index means try to add to that message if it exists, else create new message
// template <typename T>
// void append_to_bundle_msg_key_value_new(OSCBundle* bndl, char* id_header, char* key, T elem, int msg_idx)
// {
// 	// Index valid
// 	if ( (msg_idx >= 0) && (msg_idx < bndl->size()) ) {
// 		append_to_bundle_msg_key_value_new_aux(bndl, key, elem, msg_idx);
// 	} 
// 	// Add to last message
// 	else if (msg_idx == -1) {
// 		append_to_bundle_msg_key_value_new_aux(bndl, key, elem, bndl->size()-1);
// 	}
// 	// Create new or invalid index
// 	else {
// 		bndl->add(id_header);
// 		append_to_bundle_msg_key_value_new_aux(bndl, key, elem, bndl->size()-1);
// 	}
// }


// template <typename T>
// void append_to_bundle_msg_key_value_new(OSCBundle* bndl, char* id_header, char* key, T elem, int msg_idx)
// {
// 	int idx = append_determine_index(bndl, msg_idx);
// 	// // Index valid
// 	// if ( (msg_idx >= 0) && (msg_idx < bndl->size()) ) {
// 	// 	append_to_bundle_msg_key_value_new_aux(bndl, key, elem, msg_idx);
// 	// } 
// 	// // Add to last message
// 	// else if (msg_idx == -1) {
// 	// 	append_to_bundle_msg_key_value_new_aux(bndl, key, elem, bndl->size()-1);
// 	// }
// 	// // Create new or invalid index
// 	// else {
// 	// 	bndl->add(id_header);
// 	// 	append_to_bundle_msg_key_value_new_aux(bndl, key, elem, bndl->size()-1);
// 	// }
// }









// void append_to_bundle_msg_key_value(OSCBundle* bndl, char* key, uint16_t elem)
// {
// 	bndl->getOSCMessage( bndl->size()-1 )->add(key).add( (int32_t)elem );
// }

// void append_to_bundle_msg_key_value(OSCBundle* bndl, char* key, String elem)
// {
// 	bndl->getOSCMessage( bndl->size()-1 )->add(key).add( elem.c_str() );

// }


// void append_to_bundle_msg_key_value(OSCBundle* bndl, char* key, int elem)
// {
// 	bndl->getOSCMessage( bndl->size()-1 )->add(key).add( (int32_t)elem );
// }

// void append_to_bundle_msg_key_value(OSCBundle* bndl, char* key, uint16_t elem)
// {
// 	bndl->getOSCMessage( bndl->size()-1 )->add(key).add( (int32_t)elem );
// }

// void append_to_bundle_msg_key_value(OSCBundle* bndl, char* key, String elem)
// {
// 	bndl->getOSCMessage( bndl->size()-1 )->add(key).add( elem.c_str() );

// }













// void append_to_bundle_key_value_block_aux(OSCMessage *msg, int elem)
// { msg->add((int32_t)elem); }

// void append_to_bundle_key_value_block_aux(OSCMessage *msg, String elem)
// { msg->add(elem.c_str()); }

// template <typename T> 
// void append_to_bundle_key_value_block_aux(OSCMessage *msg, T elem) 
// { msg->add(elem); }

// template <typename T> 
// void append_to_bundle_key_value_block(OSCBundle *bndl, char* keys[], T elems[], int count)
// {
// 	char address_string[255];
// 	append_to_bundle_key_value_aux(bndl, address_string, "data");
// 	OSCMessage msg = OSCMessage(address_string);	

// 	for (int i = 0; i < count; i++) {
// 		msg.add( keys[i] );
// 		append_to_bundle_key_value_block_aux(&msg, elems[i]); 

// 	}
// 	bndl->add(msg);
// }











void deep_copy_message(OSCMessage *srcMsg, OSCMessage *destMsg)
{
	destMsg->empty();
	for (int i = 0; i < srcMsg->size(); i++) {
		switch (srcMsg->getType(i)) {
 			case 'i': destMsg->add(srcMsg->getInt(i));	    break;
 			case 'f': destMsg->add(srcMsg->getFloat(i));	break;
 			case 's': char buf[256];  srcMsg->getString(i, buf, 256);  destMsg->add(buf);  break;
 			default: LOOM_DEBUG_Println("Unsupported data data_type.");
 		}
	}

	char addr[50];
	srcMsg->getAddress(addr);
	destMsg->setAddress(addr);
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
	OSCMessage tmpMsg;
	for (int i = 0; i < srcBndl->size(); i++) { 	// for each message
		deep_copy_message( srcBndl->getOSCMessage(i) , &tmpMsg );
		destBndl->add(tmpMsg);
	} 
}





// --- GET NUM DATA PAIRS ---
//
// Returns the number of key-value pairs in a bundle
//  in either SINGLEMSG or MULTIMSG format
// Assumes data to be keys and values, return value
//  undefined otherwise
int bundle_num_data_pairs(OSCBundle *bndl)
{
	int tmp, total = 0;
	for (int i = 0; i < bndl->size(); i++) {
		tmp = bndl->getOSCMessage(i)->size();
		total += (tmp == 1) ? 1 : tmp/2;
	}
	return total;
}











