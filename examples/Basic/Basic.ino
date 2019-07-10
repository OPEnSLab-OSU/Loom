#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

LoomManager Manager("Manager", "Loom", 1, 1);


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
	JsonObject tmp = Manager.package();
	serializeJsonPretty(tmp, Serial);

	delay(2000);
}
