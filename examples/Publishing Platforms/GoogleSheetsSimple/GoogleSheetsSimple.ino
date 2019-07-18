
#include <Loom.h>

// Fill out the config with your google sheets values
const char* json_config_pretty = 
#include "json_config_pretty.h"
;


LoomManager Loom{};

void setup() 
{
	Loom.begin_serial(true);
	Loom.parse_config(json_config_pretty);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** "); 
}



void loop() 
{
	Loom.measure();
	Loom.package();
	Loom.print_internal_json();

	// Loom.publish_all();
	Loom.PublishPlat(0).publish();


	delay(10000);
}