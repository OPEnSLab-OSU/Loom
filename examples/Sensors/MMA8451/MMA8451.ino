///////////////////////////////////////////////////////////////////////////////

// This is a basic example to test the MMA8451 accelerometer sensor

// Documentation for MMA8451: https://openslab-osu.github.io/Loom/doxygenV2/html/class_loom___m_m_a8451.html

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
	Loom.print_config(true);

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Loom.measure();
	Loom.package();
	Loom.MMA8451().print_measurements();
	Loom.display_data();
	Loom.pause();
}
