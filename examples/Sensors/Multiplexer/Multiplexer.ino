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
	Feather.measure();
	Feather.package();
	Feather.display_data();
	Feather.pause();
}
