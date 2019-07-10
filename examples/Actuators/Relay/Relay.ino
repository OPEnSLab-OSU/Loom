#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;


LoomManager Manager("");


void setup() 
{ 
	Manager.begin_serial(true);
	Manager.parse_config(json_config);
	Manager.print_config();

	LPrintln("\n ** Setup Complete ** ");
}


void loop() 
{
	Manager.Relay().set(true);
	delay(2000);
	Manager.Relay().set(false);
	delay(2000);
}
