///////////////////////////////////////////////////////////////////////////////

// This is simple example that is used to toggle a relay on and off for 2 
// seconds each.

// The only configuration value the relay needs is the pin of the relay
// which is currently set to pin 10 in the config

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
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
	Loom.Relay().set(true);
	Loom.pause(2000);
	Loom.Relay().set(false);
	Loom.pause(2000);
}
