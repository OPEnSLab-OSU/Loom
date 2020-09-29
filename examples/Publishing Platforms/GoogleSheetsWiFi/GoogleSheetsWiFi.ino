///////////////////////////////////////////////////////////////////////////////

// This is the simplest example of logging data to Google Sheets

// The only difference between this example an 'Basic' is the LoomFactory
// settings, the line: 
//		Exec.GoogleSheets().publish();
// and the configuration, enabling logging to Google Sheets.

// In the config, you need:
// - WiFi network name and password (or '' if no password)
// - For Google sheets parameters, see:
//   https://github.com/OPEnSLab-OSU/Loom/wiki/Using-Loom-with-Google-Sheets

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In Tools menu, set:
// Internet  > WiFi
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

	Exec.get<Loom::GoogleSheets>().publish();

	Exec.pause();
}