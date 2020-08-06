///////////////////////////////////////////////////////////////////////////////

// A basic Ishield example

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In config, instance number is number to set ModuleCore to in Max MSP

// Include configuration
const char* json_config =
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Loom_Analog,
	Loom_Digital,
	Loom_WiFi,
	Loom_MaxSub,
	Loom_MaxPub,
	Loom_MPU6050
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };



void setup()
{
	Loom.begin_LED();
	Loom.begin_serial(false);
	Loom.parse_config(json_config);
	Loom.print_config(true);


	LPrintln("\n ** Setup Complete ** ");
}

void loop()
{
	Loom.measure();
	Loom.package();
	Loom.display_data();
	Loom.MaxPub().publish();
	Loom.MaxSub().subscribe();
	Loom.pause();
}
