///////////////////////////////////////////////////////////////////////////////

// This is the simplest example of logging data to Google Sheets

// The only difference between this example an 'Basic' is the LoomFactory
// settings, the line:
//		Feather.GoogleSheets().publish();
// and the configuration, enabling logging to Google Sheets.

// In the config, you need:
// - MAC address for the Ethernet module (you could also replace Ethenet with WiFi)
//		You can use 'default' instead of a parameter list for Ethenet if you
//		are not on a network that restricts to only registered MAC addresses
// - For Google sheets parameters, see:
//   https://github.com/OPEnSLab-OSU/Loom/wiki/Using-Loom-with-Google-Sheets

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config =
#include "config.h"
;

// In Tools menu, set:
// Internet  > Ethernet
// Sensors   > Enabled
// Radios    > Disabled
// Actuators > Enabled
// Max       > Disabled

using namespace Loom;

Loom::Manager Feather{};


void setup()
{
	Feather.begin_serial(true);
	Feather.parse_config(json_config);
	Feather.print_config();

	LPrintln("\n ** Setup Complete ** ");
}


void loop()
{
	Feather.measure();
	Feather.package();
	Feather.display_data();

	Feather.get<Loom::GoogleSheets>()->publish();

	Feather.pause();
}
