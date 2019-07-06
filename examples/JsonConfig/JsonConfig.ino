
#include <Loom.h>

// Define config in .ino
const char* json_config = "{\"general\":{\"device_name\":\"Device\",\"family\":\"Loom\",\"instance_num\":1,\"family_num\":0},\"components\":[{\"name\":\"Loom_Analog\",\"parameters\":[\"Analog\",8,12,true,true,true,true,true,true,0,0,0,0,0,0]},{\"name\":\"Loom_Digital\",\"parameters\":[\"Digital\",true,true,false,false,false,false,false,false,false,false,false,false]},{\"name\":\"Loom_Interrupt_Manager\",\"parameters\":[\"Interrupt-Manager\",0]},{\"name\":\"Loom_Relay\",\"parameters\":[\"Relay\",10]}]}";

// Define config in external file,
// compact and in same format as above
const char* json_config_compact = 
#include "json_config_compact.h"
;

// Define config in external file, 
// formatted for readability at size cost
const char* json_config_pretty = 
#include "json_config_pretty.h"
;


LoomManager Manager("Manager", "Loom", 1, 1, DeviceType::NODE, Verbosity::V_HIGH, Verbosity::V_LOW);



void setup() 
{
	pinMode(LED_BUILTIN, OUTPUT);   // Set the LED pin mode

	Serial.begin(115200);
	while(!Serial);       			// Ensure Serial is ready to go before anything happens in LOOM_DEBUG mode.
	delay(1000);

	LPrintln("Initialized Serial!\n");

	LPrintln("\nConfig:\n", json_config, "\n");

	// Manager.parse_config(json_config);
	// Manager.parse_config(json_config_compact);
	Manager.parse_config(json_config_pretty);
	Manager.print_config();

	Manager.Relay().set(true);

	// LPrintln("strlen pretty: ", strlen(pretty));
	LPrintln("strlen json_config: ", strlen(json_config));
	LPrintln("strlen json_config_compact: ", strlen(json_config_compact));
	LPrintln("strlen json_config_pretty ", strlen(json_config_pretty));

	LPrintln("\n ** Setup Complete ** ");
}



void loop() 
{

}


