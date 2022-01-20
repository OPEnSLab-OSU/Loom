///////////////////////////////////////////////////////////////////////////////

/// NOTE: Make sure to change the value of RH_SERIAL_MAX_PAYLOAD_LEN in RH_Serial.h
/// if you want to send large payloads.  By default, this value is not large enough
/// to use the example sketches.

// This is a basic example of receiving data via Freewave.
// While you can do a variety of things with the received data, this example
// simply prints it and logs it to an SD Card

// The corresponding example is FW > Transmit

// These two examples are the Loom equivalent of the basic RX / TX FW
// examples

// See https://openslab-osu.github.io/Loom/html/class_loom___FW.html
// for details of FW config options

// There is a similar receiving example that, unlike this one, will
// block / wait for a packet before continuing, that example is
// FW > Receive_Blocking

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
	if (getFW(Feather).receive()) {
		Feather.display_data();
		getSD(Feather).log("received.csv");
	}

	Feather.pause();	// Pause according to 'interval' in config
					// This controls the frequency of checking
					// for incoming data
}
