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

Loom::Manager Exec{};


void setup() 
{ 
	Exec.begin_serial(true);
	Exec.parse_config(LCONFIG);
	Exec.print_config();
	Exec.get<Loom::MPU6050>().print_state();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Exec.measure();
	Exec.get<Loom::MPU6050>().print_measurements();
	// Exec.package();
	// Exec.display_data();

	Exec.pause();
}
