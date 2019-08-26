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
	if ( !Loom.parse_config_SD("config3.txt") ) {
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
