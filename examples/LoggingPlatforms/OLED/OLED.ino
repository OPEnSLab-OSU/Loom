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
	Exec.get<Loom::OLED>()->log();
	Exec.pause();
}
