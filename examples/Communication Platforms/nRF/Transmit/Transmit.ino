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

// Include configuration
const char* json_config = 
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Enabled,
	Enable::Radios::Enabled,
	Enable::Actuators::Disabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };



void setup() 
{ 
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.nRF().set_print_verbosity(Verbosity::V_HIGH);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Loom.measure();
	Loom.package();
	Loom.display_data();

	// Select on of the following
	// Loom.nRF().send(01);		// Send to specific address
	Loom.nRF().broadcast();		// Broadcast to all nRF devices

	Loom.pause();	// Delay between interations set with 'interval' in config
}
