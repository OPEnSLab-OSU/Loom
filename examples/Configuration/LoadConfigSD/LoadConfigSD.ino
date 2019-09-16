///////////////////////////////////////////////////////////////////////////////

// This example demonstrates how to load a configuration from an SD card.

// Make sure your SD Featherwing or breakout is setup correctly, an SD card
// inserted and stores a configuration in a file with a name matching what is 
// used in Loom.parse_config_SD() below.

// If the device fails to load the SD configuration, it will default to the 
// configuration in the config.h file .

// Thge parse_config_SD method can also be run if you are already running a 
// different configuration, it will simply clear out the existing modules and
// instantiate new ones according to the configuration on the SD card.

// The lines:
// 		if ( !Loom.parse_config_SD("config.txt") ) {
// 			Loom.parse_config(json_config);
//		}
// are the core part of this example, and all that you would have to add to a 
// different example to add SD config support

// Put the included config.txt on the SD card.

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
// Optional if reading config from SD
// The following is what can be used if SD config not found
const char* json_config = 
#include "config.h"
;


// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Enabled,
	Enable::Radios::Enabled,
	Enable::Actuators::Enabled,
	Enable::Max::Enabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };



void setup() 
{ 
	Loom.begin_serial(true);

	// Get config from SD
	// Else use '#include'd json_config above
	if ( !Loom.parse_config_SD("config.txt") ) {
		Loom.parse_config(json_config);
	}
	
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	// Loom.measure();
	// Loom.package();
	// Loom.display_data();

	// delay(2000);
}
