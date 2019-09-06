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
	Loom.begin_serial();
	Loom.parse_config(json_config);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	if (Loom.LoRa().receive()) {
		Loom.display_data();
		Loom.SDCARD().log("received.csv");
	}

	Loom.pause();
}
