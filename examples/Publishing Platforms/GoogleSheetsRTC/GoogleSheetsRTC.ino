#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
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

	Loom.PublishPlat(0).publish();

	Loom.pause();
}