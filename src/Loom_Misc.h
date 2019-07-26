#pragma once

#include <Arduino.h>
#include "Loom_Macros.h"

///////////////////////////////////////////////////////////////////////////////
/// Print an array.
/// 3 Options
///   1: every element on different line
///   2: every element on same line
///   3: 5 elements per line
/// \param[in]	data	Array to print
/// \param[in]	len		Length of array
/// \param[in]	format	Display formatting
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

///////////////////////////////////////////////////////////////////////////////
/// @private (hide from Doxygen)
void print_array(String data [], int len, int format=1);

///////////////////////////////////////////////////////////////////////////////
/// @private (hide from Doxygen)
// bool functionRoute(const char* name);

// / @private (hide from Doxygen)
// template<typename Arg1>
// bool functionRoute(const char* name, const Arg1 arg1) { return false; } 

/// Used to call lambda function with parameters set if search string matches string associated with function.
/// Recursively get next pair of function-name, function-pointer from variadic list
/// \param[in]	name	Name of function to look for
/// \param[in]	fName	First function name to compare against
/// \param[in]	f		Function pointer
/// \param[in]	args	The rest of the list of names and pointers
// template<typename FName, typename FType, typename... Args>
// bool functionRoute(const char* name, const FName fName, const FType f, const Args... args)
// {
// 	if ( strcmp(name, fName) == 0 ) {	// Found match, call lambda that calls function
// 		// LPrintln("Found ", name, " match");
// 		f();
// 		return true;
// 	} else {	// Search the rest of the string-function pairs
// 		// LPrintln(name, " and ", fName, " did not match");
// 		return functionRoute(name, args...);
// 	}
// }

///////////////////////////////////////////////////////////////////////////////

// 