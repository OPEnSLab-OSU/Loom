
#include <Loom.h>

// Fill out the config with your google sheets values
const char* json_config = 
#include "config.h"
;


LoomManager Loom{};

void setup() 
{
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();


	LPrintln("\nEpoch: ", Loom.InternetPlat(0).get_time() );

	LPrintln("\n ** Setup Complete ** "); 
}



void loop() 
{
	Loom.measure();
	Loom.package();
	Loom.display_data();

	// Loom.publish_all();
	Loom.PublishPlat(0).publish();


	delay(10000);
}