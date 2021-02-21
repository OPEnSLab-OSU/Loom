///////////////////////////////////////////////////////////////////////////////

// This is a basic example that demonstrates how use an OLED display with 
// Loom to display data

// See the documentation for details on OLED configuration options and methods:
// https://openslab-osu.github.io/Loom/html/class_loom___o_l_e_d.html

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In Tools menu, set:
// Internet  > Disabled
// Sensors   > Enabled
// Radios    > Enabled
// Actuators > Enabled
// Max       > Enabled


using namespace Loom;

Loom::Manager Feather{};

void setup() 
{ 
	Feather.begin_LED();
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
	Feather.get<Loom::OLED>()->log();
	Feather.pause();
}
