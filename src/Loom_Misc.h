///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Misc.h
/// @brief		File for miscellaneous global Loom function declarations.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Arduino.h>
#include "Loom_Macros.h"

///////////////////////////////////////////////////////////////////////////////
/// Print an array.
/// 3 Options
///   1: every element on different line
///   2: every element on same line
///   3: 5 elements per line
/// @param[in]	data	Array to print
/// @param[in]	len		Length of array
/// @param[in]	format	Display formatting
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
