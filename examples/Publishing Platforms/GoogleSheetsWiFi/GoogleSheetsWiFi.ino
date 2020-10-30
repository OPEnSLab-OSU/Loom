///////////////////////////////////////////////////////////////////////////////

// This is the simplest example of logging data to Google Sheets

// The only difference between this example an 'Basic' is the LoomFactory
// settings, the line: 
//		Loom.GoogleSheets().publish();
// and the configuration, enabling logging to Google Sheets.

// In the config, you need:
// - WiFi network name and password (or '' if no password)
// - For Google sheets parameters, see:
//   https://github.com/OPEnSLab-OSU/Loom/wiki/Using-Loom-with-Google-Sheets

// Documentation for GoogleSheets: https://openslab-osu.github.io/Loom/doxygenV2/html/class_loom___google_sheets.html
// Documentation for WiFi: https://openslab-osu.github.io/Loom/doxygenV2/html/class_loom___wi_fi.html

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::WiFi,
	Enable::Sensors::Enabled,
	Enable::Radios::Disabled,
	Enable::Actuators::Enabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };



void setup() 
{
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** "); 
}


void loop() 
{
	Loom.measure();
	Loom.package();
	Loom.display_data();

	Loom.GoogleSheets().publish();

	Loom.pause();
}