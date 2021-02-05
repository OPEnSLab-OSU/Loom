///////////////////////////////////////////////////////////////////////////////

// This is a basic example to test the MMA8451 accelerometer sensor

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In Tools menu, set:
// Internet  > Disabled
// Sensors   > Enabled
// Radios    > Disabled
// Actuators > Disabled
// Max       > Disabled

using namespace Loom;

Loom::Manager Feather{};


void setup()
{
	Feather.begin_serial(true);
	Feather.parse_config(LCONFIG);
	Feather.print_config(true);

	LPrintln("\n ** Setup Complete ** ");
}

void loop()
{
	Feather.measure();
	Feather.package();
	Feather.get<Loom::MMA8451>()->print_measurements();
	Feather.display_data();
	Feather.pause();
}
