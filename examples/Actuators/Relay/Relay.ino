///////////////////////////////////////////////////////////////////////////////

// This is simple example that is used to toggle a relay on and off for 2
// seconds each.

// The only configuration value the relay needs is the pin of the relay
// which is currently set to pin 10 in the config

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config =
#include "config.h"
;

// In Tools menu, set:
// Internet  > Disabled
// Sensors   > Enabled
// Radios    > Disabled
// Actuators > Enabled
// Max       > Disabled

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
	getRelay(Feather).set(true);
	Feather.pause(2000);
	getRelay(Feather).set(false);
	Feather.pause(2000);
}
