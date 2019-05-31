#pragma once

#include <Arduino.h>
#include "Loom_Macros.h"


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
void replace_char(char* str, const char orig, const char rep); 





bool functionRoute(const char* name);

template<typename Arg1>
bool functionRoute(const char* name, const Arg1 arg1) { return false; } 

template<typename FName, typename FType, typename... Args>
bool functionRoute(const char* name, const FName fName, const FType f, const Args... args)
{
	if ( strcmp(name, fName) == 0 ) {	// Found match, call lambda that calls function
		// LPrintln("Found ", name, " match");
		f();
		return true;
	} else {	// Search the rest of the string-function pairs
		// LPrintln(name, " and ", fName, " did not match");
		return functionRoute(name, args...);
	}
}


