///////////////////////////////////////////////////////////////////////////////

// This is a basic example to test the MPU6050 accelerometer / gyroscope sensor

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
	Feather.print_config();
	Feather.get<Loom::MPU6050>()->print_state();

	LPrintln("\n ** Setup Complete ** ");
}

void loop()
{
	Feather.measure();
	Feather.get<Loom::MPU6050>()->print_measurements();
	// Feather.package();
	// Feather.display_data();

	Feather.pause();
}
