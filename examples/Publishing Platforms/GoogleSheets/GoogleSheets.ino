
#include <Loom.h>

// Fill out the config with your google sheets values
const char* json_config_pretty = 
#include "json_config_pretty.h"
;


// Set enabled modules
LoomFactory<
	Enable::Internet::All,
	Enable::Sensors::Enabled,
	Enable::Radios::Disabled,
	Enable::Actuators::Enabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };



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
