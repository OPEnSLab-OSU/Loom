#pragma once
#ifndef LOOM_MISC_h
#define LOOM_MISC_h

// #include "Loom_Macros.h"

#include <OSCBundle.h>



// #include <initializer_list>

#include <ArduinoJson.h>





// void func();// {} // termination version

// template<typename Arg1, typename... Args>
// void func(const Arg1& arg1, const Args&&... args)
// {
//     Serial.println( arg1 );
//     func(args...); // note: arg1 does not appear here!
// }


// Recursively extract the first two args at a time


void func();// {} // termination version

template<typename Arg1>
void func(const Arg1 arg1)
{
	Serial.println( arg1 );
}



template<typename Arg1, typename Arg2, typename... Args>
void func(const Arg1 arg1, const Arg2 arg2, const Args... args)
{
	Serial.print( arg1 );
	Serial.print(" ");
	Serial.println( arg2 );
	func(args...); // note: arg1 does not appear here!
}






/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////


void package_json(JsonObject json, const char* module_name); // termination version

template<typename Arg1>
void package_json(JsonObject json, const char* module_name, const Arg1 arg1) 
{
	Serial.print("Not enough arguments to package");
}


void package_json_aux(JsonArray data); // termination version no args

template<typename Arg1>
void package_json_aux(JsonArray data, const Arg1 arg1) {} // termination version 1 arg


// Add 1 key value pair to a compenent
template<typename Arg1, typename Arg2, typename... Args>
void package_json_aux(JsonArray data, const Arg1 arg1, const Arg2 arg2, const Args... args)
{
	JsonObject tmp = data.createNestedObject();
	tmp[arg1] = arg2;
	package_json_aux(data, args...); 
}


template<typename... Args>
void package_json(JsonObject json, const char* module_name, const Args... args)
{
	// Get / Create array for module data blocks
	JsonArray contents = json["contents"];
	if (contents.isNull()) {
		contents = json.createNestedArray("contents");
	}

	// Find module object if it exists
	JsonObject compenent;
	for (auto module_obj : contents) {
		if ( strcmp(module_obj["module"], module_name) == 0 ) {
			compenent = module_obj;
			break;
		}
	}

	// If module object does not exist yet
	// create object, specify module name, 
	// and create data array
	if (compenent.isNull()) {
		compenent = contents.createNestedObject();
		compenent["module"] = module_name;
		compenent.createNestedArray("data"); 
	}

	// Add key value pairs to module's data array
	package_json_aux(compenent["data"], args...); 
}



// This might be useful::
// void package_json_flat(JsonObject json, const char* module_name, const Args... args)





// template<typename Arg1, typename Arg2, typename... Args>
// void test_json(JsonObject json, const char* module_name, const Arg1 arg1, const Arg2 arg2, const Args... args)
// {





// 	Serial.print( arg1 );
// 	Serial.print(" ");
// 	Serial.println( arg2 );
// 	test_json_aux(json, args...); // note: arg1 does not appear here!
// }








// template<typename... Types> 
// void package_json(JsonObject json, Types... vars)
// // void package_json_data(int json)
// {

// 	// for(const auto v : {1, 2, 3}) {
// 	// 	Serial.println(v);
// 	// }

// 	// for(const auto v : {1, 2, 3}) {
// 	// 	Serial.println(v);
// 	// }
// 	LPrintln("START", vars...);
// 	serializeJson(json, Serial);


// 	JsonArray contents = json["contents"];
// 	if (contents.isNull()) {
// 		// LPrintln("Array is null, creating contents array", vars);
// 		contents = json.createNestedArray("contents");
// 	}

// 	JsonObject compenent = contents.createNestedObject();
// 	compenent["name"] = "module_name";

// 	JsonArray data = compenent.createNestedArray("data"); 
// 	JsonObject tmp;

// 	tmp = data.createNestedObject();
// 	tmp["Vbat"] = 4.37;




// 	// LPrintln("END", vars);
// 	serializeJson(json, Serial);

// }
// 




/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////





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





char get_message_type(OSCMessage* msg);
int extract_family_number(OSCMessage* msg);
int extract_family_number(OSCBundle& bndl);

int extract_device_number(OSCMessage* msg);
int extract_device_number(OSCBundle& bndl);

void extract_family(OSCMessage* msg, char* result);
void extract_family(OSCBundle& bndl, char* result);
char* extract_family(OSCMessage* msg);
char* extract_family(OSCBundle& bndl);


void extract_device(OSCMessage* msg, char* result);
void extract_device(OSCBundle& bndl, char* result);
char* extract_device(OSCMessage* msg);
char* extract_device(OSCBundle& bndl);











enum AppendType { NEW_MSG=-2, LAST_MSG=-1 };

// Char* key versions
void append_to_bundle_aux(OSCBundle& bndl, const char* key, bool elem,     int msg_idx);
void append_to_bundle_aux(OSCBundle& bndl, const char* key, int elem,      int msg_idx);
void append_to_bundle_aux(OSCBundle& bndl, const char* key, uint16_t elem, int msg_idx);
void append_to_bundle_aux(OSCBundle& bndl, const char* key, float elem,    int msg_idx);
void append_to_bundle_aux(OSCBundle& bndl, const char* key, double elem,   int msg_idx);
void append_to_bundle_aux(OSCBundle& bndl, const char* key, String elem,   int msg_idx);
template <typename T> 
void append_to_bundle_aux(OSCBundle& bndl, const char* key, T elem, int msg_idx)
{
	bndl.getOSCMessage(msg_idx)->add(key).add( elem );
}

// Int key versions
void append_to_bundle_aux(OSCBundle& bndl, int key, bool elem,      int msg_idx);
void append_to_bundle_aux(OSCBundle& bndl, int key, int elem,      int msg_idx);
void append_to_bundle_aux(OSCBundle& bndl, int key, uint16_t elem, int msg_idx);
void append_to_bundle_aux(OSCBundle& bndl, int key, float elem,    int msg_idx);
void append_to_bundle_aux(OSCBundle& bndl, int key, double elem,   int msg_idx);
void append_to_bundle_aux(OSCBundle& bndl, int key, String elem,   int msg_idx);
template <typename T> 
void append_to_bundle_aux(OSCBundle& bndl, int key, T elem, int msg_idx)
{
	bndl.getOSCMessage(msg_idx)->add( (int32_t)key ).add( elem );
}



// msg_idx of -1 means add to last message
// msg_idx of -2 means add create new message
// any other message index means try to add to that message if it exists, else create new message
template <typename T>
void append_to_bundle(OSCBundle& bndl, const char* id_header, const char* key, T elem, const int msg_idx=-1)
{
	// Index valid
	if ( (msg_idx >= 0) && (msg_idx < bndl.size()) ) {
		append_to_bundle_aux(bndl, key, elem, msg_idx);
	} 
	// Add to last message
	else if ( (msg_idx == -1) && (bndl.size() > 0) ) {
		append_to_bundle_aux(bndl, key, elem, bndl.size()-1);
	}
	// Create new or invalid index
	else {
		bndl.add(id_header);
		append_to_bundle_aux(bndl, key, elem, bndl.size()-1);
	}
}

template <typename T>
void append_to_bundle(OSCBundle& bndl, const char* id_header, const int key, T elem, const int msg_idx=-1)
{
	// Index valid
	if ( (msg_idx >= 0) && (msg_idx < bndl.size()) ) {
		append_to_bundle_aux(bndl, key, elem, msg_idx);
	} 
	// Add to last message
	else if ( (msg_idx == -1) && (bndl.size() > 0) ) {
		append_to_bundle_aux(bndl, key, elem, bndl.size()-1);
	}
	// Create new or invalid index
	else {
		bndl.add(id_header);
		append_to_bundle_aux(bndl, key, elem, bndl.size()-1);
	}
}






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
