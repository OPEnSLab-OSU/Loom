///////////////////////////////////////////////////////////////////////////////

// This is simple one of the simplest examples of using Loom. The code used
// in this example shows up in the majority of other cases as well. 

// The include, config, LoomFactory, and LoomManager are typically used in
// this way.

// The setup almost always includes what is in this example, and in many 
// cases, no further setup is necessary

// The loop is also the start of what most cases use, namely: measure,
// package, display, and wait (according to 'interval' in the config).
// Other examples simply build off of this sequence

// The configuration currently expects the relay is connected to pin 10

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
	Exec.begin_LED();
	Exec.begin_serial(true);
	Exec.parse_config(LCONFIG);
	Exec.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Exec.measure();
	Exec.package();
	Exec.display_data();
	Exec.pause();
}
