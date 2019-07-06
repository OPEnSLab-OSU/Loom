#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;


LoomManager Manager("Manager", "Loom", 1, 1, LoomManager::DeviceType::NODE, Verbosity::V_HIGH, Verbosity::V_LOW);

void setup() 
{
	pinMode(LED_BUILTIN, OUTPUT); 

	Serial.begin(115200);
	while(!Serial);
	delay(1000);

	LPrintln("Initialized Serial!\n");

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
