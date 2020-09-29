///////////////////////////////////////////////////////////////////////////////

// This is an example to use with the Loom Multiplexer to enable you to 
// dynamically connect any Loom-supported I2C sensors

// The code is essentially the same a 'Basic', the only real different is the
// enabling of the Multiplexer in the configuration.

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

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Exec.measure();
	Exec.package();
	Exec.display_data();
	Exec.pause();
}
