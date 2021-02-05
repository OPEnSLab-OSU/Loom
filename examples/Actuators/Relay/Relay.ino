///////////////////////////////////////////////////////////////////////////////

// This is simple example that is used to toggle a relay on and off for 2
// seconds each.

// The only configuration value the relay needs is the pin of the relay
// which is currently set to pin 10 in the config

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

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
	Feather.parse_config(LCONFIG);
	Feather.print_config();

	LPrintln("\n ** Setup Complete ** ");
}


void loop()
{
	Feather.get<Loom::Relay>()->set(true);
	Feather.pause(2000);
	Feather.get<Loom::Relay>()->set(false);
	Feather.pause(2000);
}
