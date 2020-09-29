///////////////////////////////////////////////////////////////////////////////

// This example is used to demonstrate the different ways to include a Json
// configuration.

// The options are
// - In the .ino file (json_config below)
// - In a separate .h file, with compact formatting (json_config_compact below)
// - In a separate .h file, with pretty formatting (json_config_pretty below)

// The size of each configuration is listed at the end of setup
// Note that the pretty formatting takes up more space in memory

// Change the variable passed to Exec.parse_config() to include a different 
// configuration

// The purpose of this example is simply the different include methods, so
// there is no code in the loop. 

// You can also load a configuration from an SD card, see the 
// Configuration >  LoadConfigSD example for how to do that

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Define config in .ino
const char* json_config = "{\"general\":{\"device_name\":\"Device\",\"family\":\"Loom\",\"instance_num\":1,\"family_num\":0},\"components\":[{\"name\":\"Loom_Analog\",\"params\":[\"Analog\",8,12,true,true,true,true,true,true,0,0,0,0,0,0]},{\"name\":\"Loom_Digital\",\"params\":[\"Digital\",true,true,false,false,false,false,false,false,false,false,false,false,25.0]},{\"name\":\"Loom_Interrupt_Manager\",\"params\":[\"Interrupt-Manager\",0]},{\"name\":\"Loom_Relay\",\"params\":[\"Relay\",10]}]}";

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

// In Tools menu, set:
// Internet  > Disabled
// Sensors   > Enabled
// Radios    > Enabled
// Actuators > Enabled
// Max       > Enabled


using namespace Loom;

Loom::Manager Exec{};


void setup() 
{
	Exec.begin_serial(true);

	LPrintln("\nConfig:\n", json_config, "\n");

	// Parse one of the configs by selecting one of the below 3 lines
	// Exec.parse_config(LCONFIG);
	// Exec.parse_config(json_config);
	// Exec.parse_config(json_config_compact);
	Exec.parse_config(json_config_pretty);

	Exec.print_config();

	LPrintln("strlen json_config: ", strlen(json_config));
	LPrintln("strlen json_config_compact: ", strlen(json_config_compact));
	LPrintln("strlen json_config_pretty ", strlen(json_config_pretty));

	LPrintln("\n ** Setup Complete ** ");
}



void loop() 
{
	// No program here, example is for how to include json config
}


