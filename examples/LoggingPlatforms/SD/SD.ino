///////////////////////////////////////////////////////////////////////////////

// This is a basic example that demonstrates how to log data to an SD card
// using Loom.

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
	
	// Log using default filename as provided in configuration
	// in this case, 'datafile.csv'
	Exec.get<Loom::SD>()->log();

	// Or log to a specific file (does not change what default file is set to)
	// Exec.get<Loom::SD>()->log("specific.csv");

	Exec.pause();	
}
