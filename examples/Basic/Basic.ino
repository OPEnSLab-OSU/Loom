#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

LoomManager Manager("");


void setup() 
{ 
	Manager.begin_LED();
	Manager.begin_serial(true);
	Manager.parse_config(json_config);
	Manager.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Manager.measure();
	Manager.package();
	Manager.print_internal_json();

	delay(2000);
}
