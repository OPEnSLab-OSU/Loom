
#include <Loom.h>

// Fill out the config with your google sheets values
const char* json_config_pretty = 
#include "json_config_pretty.h"
;


LoomManager Manager("Manager", "Loom", 1, 1, LoomManager::DeviceType::NODE, Verbosity::V_HIGH, Verbosity::V_LOW);

void setup() 
{
	pinMode(LED_BUILTIN, OUTPUT);   // Set the LED pin mode

	Serial.begin(115200);
	while(!Serial);       			// Ensure Serial is ready to go before anything happens in LOOM_DEBUG mode.
	delay(1000);

	LPrintln("Initialized Serial!\n");

	LPrintln("\nConfig:\n", json_config_pretty, "\n");
  
	Manager.parse_config(json_config_pretty);
	Manager.print_config();

	LPrintln("strlen json_config_pretty ", strlen(json_config_pretty));

	LPrintln("\n ** Setup Complete ** "); 
}



void loop() 
{
  Manager.measure();

  JsonObject j = Manager.package();

  Manager.publish(j);

  serializeJsonPretty(j, Serial);
  Serial.println();

  delay(10000);
}