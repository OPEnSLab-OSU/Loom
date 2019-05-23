#pragma once
#ifndef LOOM_MISC_h
#define LOOM_MISC_h


#include <OSCBundle.h>






String get_data_value(OSCMessage* msg, int pos);
String get_address_string(OSCMessage *msg);

void print_message(OSCMessage* msg, bool detail=true);
void print_bundle(OSCBundle& bndl);



// 3 Options
//   1: every element on different line
//   2: every element on same line
//   3: 5 elements per line
template<typename T> 
void print_array(T data [], int len, int format=1)
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

void print_array(String data [], int len, int format=1);


// Replaces substrings with other substrings in a string
// Auxiliary function for OSC string compression
void str_replace(char* target, const char* needle, const char* replacement);

// Finds nth instance of a character in a string
// Auxiliary function for OSC string compression
const char* nth_strchr(const char* s, char c, int n);

void replace_char(char* str, const char orig, const char rep); 





// // --- OSC EXTRACT HEADER SECTION ---
// //
// // Select a single part of an OSC Message header
// // Sections are separated by '/'s
// // Result does not include '/'s
// //
// // @param msg      The message to parse the header of 
// // @param section  Which section to extract (1 indexed)
// // @param result   Pointer to the char array to be filled
// //
// void osc_extract_header_section(OSCMessage* msg, int section, char* result);
// // This version works with sprintf and comparisons like strcmp, but not printing
// char* osc_extract_header_section(OSCMessage* msg, int section);



// // --- OSC EXTRACT HEADER TO SECTION ---
// //
// // Select up to and including the specified section of an osc message
// // 
// // @param msg      The message to parse the header of 
// // @param section  Which section to extract up to (inclusive) (1 indexed)
// // @param result   Pointer to the char array to be filled
// //
// void osc_extract_header_to_section(OSCMessage* msg, int section, char* result);

// // This version works with sprintf and comparisons like strcmp, but not printing
// char* osc_extract_header_to_section(OSCMessage* msg, int section);

// // --- OSC EXTRACT HEADER FROM SECTION ---  
// //
// // Select from a section of an OSC message to the end
// //
// // @param msg      The message to parse the header of 
// // @param section  Which section to start extracting from (inclusive) (1 indexed)
// // @param result   Pointer to the char array to be filled
// //
// void osc_extract_header_from_section(OSCMessage* msg, int section, char* result);

// // This version works with sprintf and comparisons like strcmp, but not printing
// char* osc_extract_header_from_section(OSCMessage* msg, int section);




// // --- OSC EXTRACT NUMBER OF SECTIONS ---
// //
// // Return the number of sections in the OSC address of 
// // an OSC message or bundle (first message)
// // 
// // @return Number of sections
// //
// int osc_address_section_count(String s);
// int osc_address_section_count(OSCMessage* msg);
// int osc_address_section_count(OSCBundle* bndl);





// char get_message_type(OSCMessage* msg);







void deep_copy_message(OSCMessage* src_msg, OSCMessage* dest_msg);

// --- DEEP COPY BUNDLE ---
// 
// Takes two bundle pointers,
// Copies the data of the first into the second
// 
// @param srcBndl   The source bundle to be copied
// @param destBndl  The bundle to copied into
//
void deep_copy_bundle(OSCBundle& src_bndl, OSCBundle& dest_bndl);


// --- GET NUM DATA PAIRS ---
//
// Returns the number of key-value pairs in a bundle
//  in either SINGLEMSG or MULTIMSG format
// Assumes data to be keys and values, return value
//  undefined otherwise
int bundle_num_data_pairs(OSCBundle& bndl);






#endif // of #ifndef LOOM_MISC_h
