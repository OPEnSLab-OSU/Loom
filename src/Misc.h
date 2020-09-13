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

// Move flags to different file
namespace Flags
{
	constexpr bool defaultTrue( bool val = true)  { return val; }
	constexpr bool defaultFalse(bool val = false) { return val; }

	constexpr bool InternetAll      = defaultTrue(false);	  ///< All internet modules enabled
	constexpr bool InternetEthernet = defaultTrue(false); ///< Only Ethernet relevant modules enabled
	constexpr bool InternetWiFi     = defaultTrue(false);	  ///< Only WiFi relevant modules enabled
	constexpr bool InternetLTE      = defaultTrue(false);	  ///< Only WiFi relevant modules enabled

	constexpr bool Sensors          = defaultTrue(false);
	constexpr bool Actuators        = defaultTrue(false);
	constexpr bool Radios           = defaultTrue(false);
	constexpr bool Max              = defaultTrue(false);

	constexpr bool Common           = true;
} // namespace Flags

///////////////////////////////////////////////////////////////////////////////

/// Different levels of verbosity (for printing or packaging)
enum class Verbosity {
	V_OFF,		///< Disable
	V_LOW, 		///< Minimal/Stardard
	V_HIGH 		///< Full details
};

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
void print_array(const T data [], const int len, const int format=1)
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
void print_array(const String data [], const int len, const int format=1);

///////////////////////////////////////////////////////////////////////////////
