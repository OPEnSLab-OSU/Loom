///////////////////////////////////////////////////////////////////////////////

// This is a basic example of receiving data via nRF. 
// While you can do a variety of things with the received data, this example
// simply prints it and logs it to an SD Card 

// The corresponding example is nRF > Transmit

// These two examples are the Loom equivalent of the basic RX / TX nRF 
// examples

// See https://openslab-osu.github.io/Loom/html/class_loom__n_r_f.html
// for details of nRF config options

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
	if (Loom.nRF().receive()) {
		Loom.display_data();
		Loom.SDCARD().log("nrf.csv");
	}

	Loom.pause();	// Pause according to 'interval' in config
					// This controls the frequency of checking
					// for incoming data

}
