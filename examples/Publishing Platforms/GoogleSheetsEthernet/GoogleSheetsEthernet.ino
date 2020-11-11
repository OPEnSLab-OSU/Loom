///////////////////////////////////////////////////////////////////////////////

// This is the simplest example of logging data to Google Sheets

// The only difference between this example an 'Basic' is the LoomFactory
// settings, the line: 
//		Exec.GoogleSheets().publish();
// and the configuration, enabling logging to Google Sheets.

// In the config, you need:
// - MAC address for the Ethernet module (you could also replace Ethenet with WiFi)
//		You can use 'default' instead of a parameter list for Ethenet if you
//		are not on a network that restricts to only registered MAC addresses
// - For Google sheets parameters, see:
//   https://github.com/OPEnSLab-OSU/Loom/wiki/Using-Loom-with-Google-Sheets

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In Tools menu, set:
// Internet  > All
// Sensors   > Enabled
// Radios    > Disabled
// Actuators > Enabled
// Max       > Disabled

using namespace Loom;

Loom::Manager Exec{};


void setup() 
{
	Exec.begin_serial(true);
	Exec.parse_config(LCONFIG);
	Exec.print_config();

	LPrintln("\n ** Setup Complete ** "); 
}


void loop() 
{
	Exec.measure();
	Exec.package();
	Exec.display_data();

	Exec.get<Loom::GoogleSheets>()->publish();

	Exec.pause();
}