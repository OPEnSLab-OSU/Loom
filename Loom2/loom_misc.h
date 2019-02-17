
#ifndef LOOM_MISC_h
#define LOOM_MISC_h

#include <OSCBundle.h>




String get_data_value(OSCMessage* msg, int pos);
String get_address_string(OSCMessage *msg);

void print_message(OSCMessage* msg, bool detail=true);
void print_bundle(OSCBundle *bndl);






// Replaces substrings with other substrings in a string
// Auxiliary function for OSC string compression
void str_replace(char *target, const char *needle, const char *replacement);

// Finds nth instance of a character in a string
// Auxiliary function for OSC string compression
const char* nth_strchr(const char* s, char c, int n);

void replace_char(char *str, char orig, char rep); 


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
void osc_extract_header_section(OSCMessage* msg, int section, char* result);
// This version works with sprintf and comparisons like strcmp, but not printing
char* osc_extract_header_section(OSCMessage* msg, int section);



// --- OSC EXTRACT HEADER TO SECTION ---
//
// Select up to and including the specified section of an osc message
// 
// @param msg      The message to parse the header of 
// @param section  Which section to extract up to (inclusive) (1 indexed)
// @param result   Pointer to the char array to be filled
//
void osc_extract_header_to_section(OSCMessage* msg, int section, char* result);

// This version works with sprintf and comparisons like strcmp, but not printing
char* osc_extract_header_to_section(OSCMessage* msg, int section);

// --- OSC EXTRACT HEADER FROM SECTION ---  
//
// Select from a section of an OSC message to the end
//
// @param msg      The message to parse the header of 
// @param section  Which section to start extracting from (inclusive) (1 indexed)
// @param result   Pointer to the char array to be filled
//
void osc_extract_header_from_section(OSCMessage* msg, int section, char* result);

// This version works with sprintf and comparisons like strcmp, but not printing
char* osc_extract_header_from_section(OSCMessage* msg, int section);
// --- OSC EXTRACT NUMBER OF SECTIONS ---
//
// Return the number of sections in the OSC address of 
// an OSC message or bundle (first message)
// 
// @return Number of sections
//
int osc_address_section_count(String s);

int osc_address_section_count(OSCMessage* msg);
int osc_address_section_count(OSCBundle* bndl);





// add the char* return versions here


char get_message_type(OSCMessage* msg);
int extract_family_number(OSCMessage* msg);
int extract_family_number(OSCBundle* bndl);

int extract_device_number(OSCMessage* msg);
int extract_device_number(OSCBundle* bndl);

void extract_family(OSCMessage* msg, char* result);
void extract_family(OSCBundle* bndl, char* result);
char* extract_family(OSCMessage* msg);
char* extract_family(OSCBundle* bndl);


void extract_device(OSCMessage* msg, char* result);
void extract_device(OSCBundle* bndl, char* result);
char* extract_device(OSCMessage* msg);
char* extract_device(OSCBundle* bndl);











enum AppendType { NEW_MSG=-2, LAST_MSG=-1 };

// Char* key versions
void append_to_bundle_aux(OSCBundle* bndl, char* key, int elem, int msg_idx);
void append_to_bundle_aux(OSCBundle* bndl, char* key, uint16_t elem, int msg_idx);
void append_to_bundle_aux(OSCBundle* bndl, char* key, float elem, int msg_idx);
void append_to_bundle_aux(OSCBundle* bndl, char* key, String elem, int msg_idx);
template <typename T> 
void append_to_bundle_aux(OSCBundle* bndl, char* key, T elem, int msg_idx)
{
	bndl->getOSCMessage(msg_idx)->add(key).add( elem );
}

// Int key versions
void append_to_bundle_aux(OSCBundle* bndl, int key, int elem, int message_index);
void append_to_bundle_aux(OSCBundle* bndl, int key, uint16_t elem, int message_index);
void append_to_bundle_aux(OSCBundle* bndl, int key, float elem, int msg_idx);
void append_to_bundle_aux(OSCBundle* bndl, int key, String elem, int msg_idx);
template <typename T> 
void append_to_bundle_aux(OSCBundle* bndl, int key, T elem, int msg_idx)
{
	bndl->getOSCMessage(msg_idx)->add( (int32_t)key ).add( elem );
}



// msg_idx of -1 means add to last message
// msg_idx of -2 means add create new message
// any other message index means try to add to that message if it exists, else create new message
template <typename T>
void append_to_bundle(OSCBundle* bndl, char* id_header, char* key, T elem, int msg_idx=-1)
{
	// Index valid
	if ( (msg_idx >= 0) && (msg_idx < bndl->size()) ) {
		append_to_bundle_aux(bndl, key, elem, msg_idx);
	} 
	// Add to last message
	else if (msg_idx == -1) {
		append_to_bundle_aux(bndl, key, elem, bndl->size()-1);
	}
	// Create new or invalid index
	else {
		bndl->add(id_header);
		append_to_bundle_aux(bndl, key, elem, bndl->size()-1);
	}
}

template <typename T>
void append_to_bundle(OSCBundle* bndl, char* id_header, int key, T elem, int msg_idx=-1)
{
	// Index valid
	if ( (msg_idx >= 0) && (msg_idx < bndl->size()) ) {
		append_to_bundle_aux(bndl, key, elem, msg_idx);
	} 
	// Add to last message
	else if (msg_idx == -1) {
		append_to_bundle_aux(bndl, key, elem, bndl->size()-1);
	}
	// Create new or invalid index
	else {
		bndl->add(id_header);
		append_to_bundle_aux(bndl, key, elem, bndl->size()-1);
	}
}






void deep_copy_message(OSCMessage *srcMsg, OSCMessage *destMsg);

// --- DEEP COPY BUNDLE ---
// 
// Takes two bundle pointers,
// Copies the data of the first into the second
// 
// @param srcBndl   The source bundle to be copied
// @param destBndl  The bundle to copied into
//
void deep_copy_bundle(OSCBundle *srcBndl, OSCBundle *destBndl);


// --- GET NUM DATA PAIRS ---
//
// Returns the number of key-value pairs in a bundle
//  in either SINGLEMSG or MULTIMSG format
// Assumes data to be keys and values, return value
//  undefined otherwise
int bundle_num_data_pairs(OSCBundle *bndl);






#endif // of #ifndef LOOM_MISC_h
