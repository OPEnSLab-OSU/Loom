///////////////////////////////////////////////////////////////////////////////

// This is a basic example that demonstrates how to log data to an SD card
// using Loom.

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config =
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Loom_SD,
	Loom_Analog,
	Loom_PCF8523
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };



void setup()
{
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
}


void loop()
{
	Loom.measure();
	Loom.package();
	Loom.display_data();

	// Log using default filename as provided in configuration
	// in this case, 'datafile.csv'
	Loom.SD().log();

	// Or log to a specific file (does not change what default file is set to)
	// Loom.SD().log("specific.csv");

	Loom.pause();
}
