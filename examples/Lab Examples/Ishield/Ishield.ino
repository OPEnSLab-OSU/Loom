///////////////////////////////////////////////////////////////////////////////

// A basic Ishield example

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In config, instance number is number to set ModuleCore to in Max MSP
// Include configuration
const char* json_config =
#include "config.h"
;

// In Tools menu, set:
// Internet  > WiFi
// Sensors   > Enabled
// Radios    > Disabled
// Actuators > Disabled
// Max       > Enabled

using namespace Loom;

Loom::Manager Feather{};


void setup()
{
	Feather.begin_LED();
	Feather.begin_serial(false);
	Feather.parse_config(json_config);
	Feather.print_config(true);

	LPrintln("\n ** Setup Complete ** ");
}

void loop()
{
	Feather.measure();
	Feather.package();
	Feather.display_data();
	getMaxPub(Feather).publish();
	getMaxSub(Feather).subscribe();
	Feather.pause();
}
