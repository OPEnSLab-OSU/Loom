///////////////////////////////////////////////////////////////////////////////

// This example is used to demonstrate how to specify modules to use default
// configuration parameters in the config (see config.h file)

// Other than that, this example is largely the same as the 'Basic' example

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
// Max       > Disabled

using namespace Loom;

Loom::Manager Feather{};


void setup()
{
	Feather.begin_serial(true);
	Feather.parse_config(json_config);
	Feather.print_config(true);

	LPrintln("\n ** Setup Complete ** ");
}

void loop()
{
	Feather.measure();
	Feather.package();
	Feather.display_data();

	Feather.pause();
}
