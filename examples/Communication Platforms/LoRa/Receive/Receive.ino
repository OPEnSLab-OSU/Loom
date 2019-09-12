///////////////////////////////////////////////////////////////////////////////

// This is a basic example of receiving data via LoRa. 
// While you can do a variety of things with the received data, this example
// simply prints it and logs it to an SD Card 

// The corresponding example is LoRa > Transmit

// These two examples are the Loom equivalent of the basic RX / TX LoRa 
// examples

// See https://openslab-osu.github.io/Loom/html/class_loom___lo_ra.html
// for details of LoRa config options

// There is a similar receiving example that, unlike this one, will 
// block / wait for a packet before continuing, that example is 
// LoRa > Receive_Blocking

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
	Loom.begin_serial();
	Loom.parse_config(json_config);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	if (Loom.LoRa().receive()) {
		Loom.display_data();
		Loom.SDCARD().log("received.csv");
	}

	Loom.pause();	// Pause according to 'interval' in config
					// This controls the frequency of checking
					// for incoming data
}
