///////////////////////////////////////////////////////////////////////////////

// This is a basic example that demonstrates how to log data to an SD card
// using Loom.

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config =
#include "config.h"
;

// In Tools menu, set:
// Internet  > Disabled
// Sensors   > Enabled
// Radios    > Enabled
// Actuators > Enabled
// Max       > Enabled

using namespace Loom;

Loom::Manager Feather{};


void setup()
{
	Feather.begin_serial(true);
	Feather.parse_config(json_config);
	Feather.print_config();

	LPrintln("\n ** Setup Complete ** ");
}


void loop()
{
	Feather.measure();
	Feather.package();
	Feather.display_data();

	// Log using default filename as provided in configuration
	// in this case, 'datafile.csv'
	getSD(Feather).log();

	// Or log to a specific file (does not change what default file is set to)
	// Feather.get<Loom::SD>()->log("specific.csv");

	Feather.pause();
}
