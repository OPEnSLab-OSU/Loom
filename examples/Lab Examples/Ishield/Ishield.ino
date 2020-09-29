///////////////////////////////////////////////////////////////////////////////

// A basic Ishield example

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In config, instance number is number to set ModuleCore to in Max MSP

// In Tools menu, set:
// Internet  > WiFi
// Sensors   > Enabled
// Radios    > Enabled
// Actuators > Enabled
// Max       > Enabled

using namespace Loom;

Loom::Manager Exec{};


void setup() 
{ 
	Exec.begin_LED();
	Exec.begin_serial(false);
	Exec.parse_config(LCONFIG);
	Exec.print_config(true);

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Exec.measure();
	Exec.package();
	Exec.display_data();
	Exec.get<Loom::MaxPub>().publish();
	Exec.get<Loom::MaxSub>().subscribe();
	Exec.pause();
}
