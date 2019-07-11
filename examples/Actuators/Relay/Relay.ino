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
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
}


void loop() 
{
	Loom.Relay().set(true);
	delay(2000);
	Loom.Relay().set(false);
	delay(2000);
}
