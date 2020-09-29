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

Loom::Manager Exec{};


void setup() 
{ 
	Exec.begin_serial(true);
	Exec.parse_config(LCONFIG);
	Exec.print_config();

	LPrintln("\n ** Setup Complete ** ");
}


void loop() 
{
	Exec.get<Loom::Relay>().set(true);
	Exec.pause(2000);
	Exec.get<Loom::Relay>().set(false);
	Exec.pause(2000);
}
