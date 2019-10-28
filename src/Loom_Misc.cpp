///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Misc.cpp
/// @brief		File for miscellaneous global Loom function definitions.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Loom_Misc.h"

///////////////////////////////////////////////////////////////////////////////
void print_array(const String data [], const int len, const int format)
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

///////////////////////////////////////////////////////////////////////////////

bool Flags::InternetAll = false;				///< All internet modules enabled
bool Flags::InternetEthernet = false;		///< Only Ethernet relevant modules enabled
bool Flags::InternetWiFi = false;				///< Only WiFi relevant modules enabled

bool Flags::Sensors = false;
bool Flags::Actuators = false;
bool Flags::Radios = false;
bool Flags::Max = false;
bool Flags::Common = false;