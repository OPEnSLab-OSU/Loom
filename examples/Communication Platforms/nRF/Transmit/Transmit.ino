///////////////////////////////////////////////////////////////////////////////

// This is a basic example of sending data via nRF. 

// The corresponding example is nRF > Receive (or Receive_Blocking)

// These two examples are the Loom equivalent of the basic RX / TX nRF 
// examples

// See https://openslab-osu.github.io/Loom/html/class_loom__n_r_f.html
// for details of nRF config options

// While you can send whatever data you collect, for the purposes of this 
// example, only analog data is being collected

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In Tools menu, set:
// Internet  > Disabled
// Sensors   > Enabled
// Radios    > Enabled
// Actuators > Disabled
// Max       > Disabled


using namespace Loom;

Loom::Manager Feather{};


void setup() 
{ 
	Feather.begin_serial(true);
	Feather.parse_config(LCONFIG);
	Feather.get<Loom::nRF>()->set_print_verbosity(Verbosity::V_HIGH);
	Feather.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Feather.measure();
	Feather.package();
	Feather.display_data();

	// Select on of the following
	// Feather.nRF().send(01);		// Send to specific address
	Feather.get<Loom::nRF>()->broadcast(); // Broadcast to all nRF devices

	Feather.pause();	// Delay between interations set with 'interval' in config
}
