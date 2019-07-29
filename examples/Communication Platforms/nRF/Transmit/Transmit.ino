#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

LoomManager Loom("");


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
	Loom.print_internal_json();

	// Sent to specific address
	// Loom.nRF().send(01);
	Loom.nRF().broadcast();

	delay(2000);
}
