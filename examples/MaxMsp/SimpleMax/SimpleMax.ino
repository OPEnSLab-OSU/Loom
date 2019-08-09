#include <Loom.h>

// In config, instance number is number to set ModuleCore to in Max MSP

// Include configuration
const char* json_config = 
#include "config.h"
;

LoomManager Loom{};


void setup() 
{ 
	Loom.begin_LED();
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config(true);

	// Loom.MaxSub().set_print_verbosity(Verbosity::V_HIGH);
	// Loom.MaxPub().set_print_verbosity(Verbosity::V_HIGH);
	
	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Loom.measure();
	Loom.package();
	Loom.display_data();
	Loom.MaxPub().publish();
	Loom.MaxSub().subscribe();
	delay(500);
}
