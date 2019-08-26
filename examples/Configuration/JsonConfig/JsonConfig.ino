
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


// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Enabled,
	Enable::Radios::Enabled,
	Enable::Actuators::Enabled,
	Enable::Max::Enabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };



void setup() 
{
	Loom.begin_serial(true);

	LPrintln("\nConfig:\n", json_config, "\n");

	// Parse one of the configs by selecting one of the below 3 lines
	// Loom.parse_config(json_config);
	// Loom.parse_config(json_config_compact);
	Loom.parse_config(json_config_pretty);

	Loom.print_config();

	LPrintln("strlen json_config: ", strlen(json_config));
	LPrintln("strlen json_config_compact: ", strlen(json_config_compact));
	LPrintln("strlen json_config_pretty ", strlen(json_config_pretty));

	LPrintln("\n ** Setup Complete ** ");
}



void loop() 
{
	// No program here, example is for how to include json config
}


