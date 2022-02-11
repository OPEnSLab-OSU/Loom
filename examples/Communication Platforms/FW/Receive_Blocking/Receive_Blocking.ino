///////////////////////////////////////////////////////////////////////////////

/// NOTE: Make sure to change the value of RH_SERIAL_MAX_PAYLOAD_LEN in RH_Serial.h
/// if you want to send large payloads.  By default, this value is not large enough
/// to use the example sketches.

// This is a basic example of receiving data via Freewave.
// This particular example blocks while waiting for data
// While you can do a variety of things with the received data, this example
// simply prints it and logs it to an SD Card

// The corresponding example is FW > Transmit

// See https://openslab-osu.github.io/Loom/html/class_loom___FW.html
// for details of FW config options

// There is a similar receiving example that, unlike this one, will not
// block / wait for a packet before continuing, that example is
// FW > Receive

// The maximum time to wait for a packet is provided in milliseconds,
// 5000 in this case

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
	if (getFW(Feather).receive_blocking(5000)) {
		Feather.display_data();
		getSD(Feather).log("received.csv");
	}
}