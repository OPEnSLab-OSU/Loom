///////////////////////////////////////////////////////////////////////////////

// This is a basic example to test the MMA8451 accelerometer sensor

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config =
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Loom_Analog,
	Loom_MMA8451
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };



void setup()
{
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config(true);

	LPrintln("\n ** Setup Complete ** ");
}

void loop()
{
	Loom.measure();
	Loom.package();
	Loom.MMA8451().print_measurements();
	Loom.display_data();
	Loom.pause();
}
