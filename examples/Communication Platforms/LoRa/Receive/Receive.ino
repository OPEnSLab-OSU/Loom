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
	Feather.begin_serial();
	Feather.parse_config(json_config);
	Feather.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop()
{
	if (getLoRa(Feather).receive()) {
		Feather.display_data();
		getSD(Feather).log("received.csv");
	}

	Feather.pause();	// Pause according to 'interval' in config
					// This controls the frequency of checking
					// for incoming data
}
