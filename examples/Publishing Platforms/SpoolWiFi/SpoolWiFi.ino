///////////////////////////////////////////////////////////////////////////////

// This is the simplest example of logging data to Spool

// The only difference between this example an 'Basic' is the LoomFactory
// settings, the line: 
//		Loom.Spool().publish();
// and the configuration, enabling logging to Spool.

// Spool is data logging and device management platform for Loom

// In the config, you need:
// - WiFi network name and password (or '' if no password)
// - For Spool parameters, see:
//   https://github.com/OPEnSLab-OSU/Loom/wiki/Using-Loom-with-Spool
//   https://github.com/OPEnSLab-OSU/Spool/wiki

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

	Loom.Spool().publish();

	Loom.pause();
}