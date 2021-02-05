///////////////////////////////////////////////////////////////////////////////

// A basic Ishield example

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In config, instance number is number to set ModuleCore to in Max MSP

// In Tools menu, set:
// Internet  > WiFi
// Sensors   > Enabled
// Radios    > Enabled
// Actuators > Enabled
// Max       > Enabled

using namespace Loom;

Loom::Manager Feather{};


void setup()
{
	Feather.begin_LED();
	Feather.begin_serial(false);
	Feather.parse_config(LCONFIG);
	Feather.print_config(true);

	LPrintln("\n ** Setup Complete ** ");
}

void loop()
{
	Feather.measure();
	Feather.package();
	Feather.display_data();
	Feather.get<Loom::MaxPub>()->publish();
	Feather.get<Loom::MaxSub>()->subscribe();
	Feather.pause();
}
