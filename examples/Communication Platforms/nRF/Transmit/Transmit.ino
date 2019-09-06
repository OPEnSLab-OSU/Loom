#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Enabled,
	Enable::Radios::Enabled,
	Enable::Actuators::Disabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };



void setup() 
{ 
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.nRF().set_print_verbosity(Verbosity::V_HIGH);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Loom.measure();
	Loom.package();
	Loom.display_data();

	// Sent to specific address
	// Loom.nRF().send(01);
	Loom.nRF().broadcast();

	Loom.pause();
}
