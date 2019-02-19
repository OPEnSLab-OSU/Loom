
#include "Loom_Misc.h"
#include "Loom_Macros.h"



void print_array(String data [], int len, int format)
{
	#if LOOM_DEBUG == 1
		for (int i = 0; i < len; i++) {
			if (format == 1) { Serial.println(data[i].c_str()); }
			if (format  > 1) { Serial.print(data[i].c_str()); Serial.print(" "); }
			if ((format > 2) && (i%5==0)) { Serial.println(); }
		}
		Serial.println();
	#endif
}

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
				Println("Unsupported data data_type.");
				return String("");
		}
	}
	Println2("Message does not have an argument at position: ", pos);

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
			Print(get_address_string(msg).c_str());
			for (int i = 0; i < msg->size(); i++) {
				Print("  ");
				switch(msg->getType(i)) {
					case 'f': Print(msg->getFloat(i) ); break;
					case 'i': Print(msg->getInt(i)   ); break;
					case 's': msg->getString(i, buf, 50); Print(buf); break;
					default: break;
				}
			}
			Println();
		} else {
			Println2("\tAddress: ", get_address_string(msg).c_str());
			for (int i = 0; i < msg->size(); i++) {
				Print3("\tValue (", i, ") ");
				switch(msg->getType(i)) {
					case 'f': Println2("(f) ", msg->getFloat(i) ); break;
					case 'i': Println2("(i) ", msg->getInt(i) );   break;
					case 's': msg->getString(i, buf, 50); Println2("(s) ", buf ); break;
					default: break;
				}
			}
		}
	#endif // of 'LOOM_DEBUG == 1'
}


void print_bundle(OSCBundle& bndl)
{
	if ( !bndl.size() ) return;

	char buf[50];
	char data_type;
	Println2("\nBundle Size: ", bndl.size());
	OSCMessage *msg;

	for (int i = 0; i < bndl.size(); i++) {
		Println2("Message: ", i);
		print_message(bndl.getOSCMessage(i));
	}
	Println();
}







// Replaces substrings with other substrings in a string
// Auxiliary function for OSC string compression
void str_replace(char* target, const char* needle, const char* replacement)
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
void replace_char(char* str, const char orig, const char rep) 
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
	for (int i = 0; i < s.length(); i++) { 
		if (s[i] == '/') count++;
	}

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

int extract_family_number(OSCBundle& bndl)
{ 
	return extract_family_number(bndl.getOSCMessage(0)); 
}



int extract_device_number(OSCMessage* msg)
{
	if ( get_message_type(msg) == 'D' ) {
		return (int)strtol( osc_extract_header_section(msg, 5) , NULL, 10);
	} else {
		return -1;
	}
}

int extract_device_number(OSCBundle& bndl)
{ 
	return extract_device_number(bndl.getOSCMessage(0)); 
}



void extract_family(OSCMessage* msg, char* result)
{ 
	osc_extract_header_section(msg, 2, result); 
}

void extract_family(OSCBundle& bndl, char* result)
{ 
	extract_family(bndl.getOSCMessage(0), result); 
}

char* extract_family(OSCMessage* msg)
{
	char result[50];
	extract_family(msg, result);
	return (char*)result;
}

char* extract_family(OSCBundle& bndl)
{ 
	return extract_family(bndl.getOSCMessage(0)); 
}




void extract_device(OSCMessage* msg, char* result)
{
	if ( get_message_type(msg) == 'D' ) {
		osc_extract_header_section(msg, 4, result);
	} else {
		sprintf(result, "\0");
	}
}

void extract_device(OSCBundle& bndl, char* result)
{ 
	extract_device(bndl.getOSCMessage(0), result); 
}

char* extract_device(OSCMessage* msg)
{
	char result[50];
	extract_device(msg, result);
	return (char*)result;
}

char* extract_device(OSCBundle& bndl)
{ 
	return extract_device(bndl.getOSCMessage(0)); 
}








void append_to_bundle_aux(OSCBundle& bndl, const char* key, int elem, int msg_idx)
{ bndl.getOSCMessage(msg_idx)->add(key).add( (int32_t)elem ); }

void append_to_bundle_aux(OSCBundle& bndl, const char* key, uint16_t elem, int msg_idx)
{ bndl.getOSCMessage(msg_idx)->add(key).add( (int32_t)elem ); }

void append_to_bundle_aux(OSCBundle& bndl, const char* key, float elem, int msg_idx)
{ bndl.getOSCMessage(msg_idx)->add(key).add( elem ); }

void append_to_bundle_aux(OSCBundle& bndl, const char* key, double elem, int msg_idx)
{ bndl.getOSCMessage(msg_idx)->add(key).add( (float)elem ); }

void append_to_bundle_aux(OSCBundle& bndl, const char* key, String elem, int msg_idx)
{ bndl.getOSCMessage(msg_idx)->add(key).add( elem.c_str() ); }

void append_to_bundle_aux(OSCBundle& bndl, int key, int elem, int msg_idx)
{ bndl.getOSCMessage(msg_idx)->add( (int32_t)key ).add( (int32_t)elem ); }

void append_to_bundle_aux(OSCBundle& bndl, int key, uint16_t elem, int msg_idx)
{ bndl.getOSCMessage(msg_idx)->add( (int32_t)key ).add( (int32_t)elem ); }

void append_to_bundle_aux(OSCBundle& bndl, int key, float elem, int msg_idx)
{ bndl.getOSCMessage(msg_idx)->add( (int32_t)key ).add( elem ); }

void append_to_bundle_aux(OSCBundle& bndl, int key, double elem, int msg_idx)
{ bndl.getOSCMessage(msg_idx)->add( (int32_t)key ).add( (float)elem ); }

void append_to_bundle_aux(OSCBundle& bndl, int key, String elem, int msg_idx)
{ bndl.getOSCMessage(msg_idx)->add( (int32_t)key ).add( elem.c_str() ); }







void deep_copy_message(OSCMessage* src_msg, OSCMessage* dest_msg)
{
	dest_msg->empty();
	for (int i = 0; i < src_msg->size(); i++) {
		switch (src_msg->getType(i)) {
 			case 'i': dest_msg->add(src_msg->getInt(i));	    break;
 			case 'f': dest_msg->add(src_msg->getFloat(i));	break;
 			case 's': char buf[256];  src_msg->getString(i, buf, 256);  dest_msg->add(buf);  break;
 			default: Println("Unsupported data data_type.");
 		}
	}

	char addr[50];
	src_msg->getAddress(addr);
	dest_msg->setAddress(addr);
}



// --- DEEP COPY BUNDLE ---
// 
// Takes two bundle pointers,
// Copies the data of the first into the second
// 
// @param srcBndl   The source bundle to be copied
// @param destBndl  The bundle to copied into
//
void deep_copy_bundle(OSCBundle& src_bndl, OSCBundle& dest_bndl) 
{
	dest_bndl.empty();
	OSCMessage tmpMsg;
	for (int i = 0; i < src_bndl.size(); i++) { 	// for each message
		deep_copy_message( src_bndl.getOSCMessage(i) , &tmpMsg );
		dest_bndl.add(tmpMsg);
	} 
}



// --- GET NUM DATA PAIRS ---
//
// Returns the number of key-value pairs in a bundle
// Assumes data to be keys and values, return value
//  undefined otherwise
int bundle_num_data_pairs(OSCBundle& bndl)
{
	int tmp, total = 0;
	for (int i = 0; i < bndl.size(); i++) {
		tmp = bndl.getOSCMessage(i)->size();
		total += (tmp == 1) ? 1 : tmp/2;
	}
	return total;
}











