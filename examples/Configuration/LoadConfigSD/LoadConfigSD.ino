///////////////////////////////////////////////////////////////////////////////

// This example demonstrates how to load a configuration from an SD card.

// Make sure your SD Featherwing or breakout is setup correctly, an SD card
// inserted and stores a configuration in a file with a name matching what is
// used in Feather.parse_config_SD() below.

// If the device fails to load the SD configuration, it will default to the
// configuration in the config.h file .

// Thge parse_config_SD method can also be run if you are already running a
// different configuration, it will simply clear out the existing modules and
// instantiate new ones according to the configuration on the SD card.

// The lines:
// 		if ( !Feather.parse_config_SD("config.txt") ) {
// 			Feather.parse_config(json_config);
//		}
// are the core part of this example, and all that you would have to add to a
// different example to add SD config support

// Put the included config.txt on the SD card.

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>


// Include configuration
const char* json_config =
#include "config.h"
;
// json_config is used if SD config not found


// In Tools menu, set:
// Internet  > Disabled
// Sensors   > Enabled
// Radios    > Enabled
// Actuators > Enabled
// Max       > Disabled

using namespace Loom;

Loom::Manager Feather{};


void setup()
{
	Feather.begin_serial(true);

	// Get config from SD
	// Else use json_config
	if ( !Feather.parse_config_SD("config.txt") ) {
		Feather.parse_config(json_config);
	}

	Feather.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop()
{
	Feather.measure();
	Feather.package();
	Feather.display_data();

	delay(2000);
}
