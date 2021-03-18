///////////////////////////////////////////////////////////////////////////////

// A basic example of running Loom with WiFi in access point mode. 
// Typically for use with Max

///////////////////////////////////////////////////////////////////////////////

#include <LoomForMax.h>

// In config, instance number is number to set ModuleCore to in Max MSP

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
	Enable::Max::Enabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };



void setup() 
{ 
	Loom.begin_LED();
	Loom.begin_serial(false);
	Loom.parse_config(json_config);
	Loom.print_config(true);

	
	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Loom.measure();
	Loom.package();
	Loom.display_data();
	Loom.MaxPub().publish();
	Loom.MaxSub().subscribe();
	Loom.pause();
}
