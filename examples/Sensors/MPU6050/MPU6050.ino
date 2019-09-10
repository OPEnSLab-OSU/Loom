///////////////////////////////////////////////////////////////////////////////

// This is a basic example to test the MPU6050 accelerometer / gyroscope sensor

///////////////////////////////////////////////////////////////////////////////


#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Enabled,
	Enable::Radios::Disabled,
	Enable::Actuators::Disabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };



void setup() 
{ 
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();
	Loom.MPU6050().print_state();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Loom.measure();
	Loom.MPU6050().print_measurements();
	// Loom.package();
	// Loom.display_data();

	Loom.pause();
}
