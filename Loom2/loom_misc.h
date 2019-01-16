
#ifndef LOOM_MISC_h
#define LOOM_MISC_h


#include <OSCBundle.h>





// enum Verbosity { VERB_OFF, VERB_LOW, VERB_HIGH }; // Move back to "loom_abstract_module_class.h"
// enum CommScope { SCOPE_DEVICE, SCOPE_SUBNET, SCOPE_FAMILY, SCOPE_SUBNET_ONLY, SCOPE_FAMILY_ONLY, SCOPE_ANY }; // Move back to "loom_comm_plat_class.h"
// enum DeviceType { HUB, NODE, REPEATER };
// enum LogPlatform { SDCARD, PUSHINGBOX, OLED, SERIAL_MON };
// enum OLED_Version { FEATHERWING, BREAKOUT, OTHER };
// enum OLED_Format { OLED_4, OLED_8, OLED_SCROLL };
// enum OLED_Freeze { FREEZE_DISABLE, FREEZE_DATA, FREEZE_SCROLL };


// Move these to the files where the enums are defined (the associated class)

// These could also be #defined / const arrays? then just use associated index

// char* enum_verbosity_string(Verbosity v);

// char* enum_subnet_scope_string(CommScope s);

// char* enum_device_type_string(DeviceType t);

// char* enum_log_plat_string(LogPlatform p);
// char* enum_oled_version_string(OLED_Version v);

// char* enum_oled_format(OLED_Format f);

// char* enum_oled_freeze(OLED_Freeze f);







String get_data_value(OSCMessage* msg, int pos);


String get_address_string(OSCMessage *msg);



void print_message(OSCMessage* msg, bool detail);

void print_message(OSCMessage* msg);
void print_bundle(OSCBundle *bndl);








// Replaces substrings with other substrings in a string
// Auxiliary function for OSC string compression
void str_replace(char *target, const char *needle, const char *replacement);

// Finds nth instance of a character in a string
// Auxiliary function for OSC string compression
const char* nth_strchr(const char* s, char c, int n);

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
//



void append_to_bundle_key_value(OSCBundle *bndl, char* id_header, char* key, int elem);
void append_to_bundle_key_value(OSCBundle *bndl, char* id_header, char* key, uint16_t elem);
// void append_to_bundle_key_value(OSCBundle *bndl, char* id_header, char* key, float elem);
void append_to_bundle_key_value(OSCBundle *bndl, char* id_header, char* key, String elem);

template <typename T> 
void append_to_bundle_key_value(OSCBundle *bndl, char* id_header, char* key, T elem)
{
	char address_string[80];
	sprintf(address_string, "%s/%s", id_header, key);

	bndl->add(address_string).add( elem );
}



// Appends to the last message in the bundle

void append_to_bundle_msg_key_value(OSCBundle* bndl, char* key, int elem);
void append_to_bundle_msg_key_value(OSCBundle* bndl, char* key, uint16_t elem);
void append_to_bundle_msg_key_value(OSCBundle* bndl, char* key, String elem);

template <typename T> 
void append_to_bundle_msg_key_value(OSCBundle* bndl, char* key, T elem)
{
	// char address_string[80];
	// sprintf(address_string, "%s/%s", id_header, key);

	// bndl->add(address_string).add( elem );

///

	// char address_string[80];
	// sprintf(address_string, "%s/Port%d%s", id_header, port, "/tsl2591/data");
	
	// OSCMessage msg = OSCMessage(address_string);
	// msg.add("vis" ).add((int32_t)state_tsl2591.vis);
	// msg.add("ir"  ).add((int32_t)state_tsl2591.ir);
	// msg.add("full").add((int32_t)state_tsl2591.full);
	
	// bndl->add(msg);

	
	// Get last message
	// OSCMessage* msg = bndl->getOSCMessage( bndl->size()-1 );

	bndl->getOSCMessage( bndl->size()-1 )->add(key).add(elem);

}


// void append_to_message_key_value(OSCMessage* msg, char* id_header, char* key, int elem);
// void append_to_message_key_value(OSCMessage* msg, char* id_header, char* key, uint16_t elem);
// void append_to_message_key_value(OSCMessage* msg, char* id_header, char* key, String elem);

// template <typename T> 
// void append_to_message_key_value(OSCMessage* msg, char* id_header, char* key, T elem)
// {
// 	char address_string[80];
// 	sprintf(address_string, "%s/%s", id_header, key);

// 	bndl->add(address_string).add( elem );
// }


// template <typename T> 
// extern void append_to_bundle_key_value(OSCBundle *bndl, char* id_header, char* key, T elem);



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
