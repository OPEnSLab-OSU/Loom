///////////////////////////////////////////////////////////////////////////////

// This is a basic example of receiving data via LoRa.
// This particular example blocks while waiting for data
// While you can do a variety of things with the received data, this example
// simply prints it and logs it to an SD Card

// The corresponding example is LoRa > Transmit

// See https://openslab-osu.github.io/Loom/html/class_loom___lo_ra.html
// for details of LoRa config options

// There is a similar receiving example that, unlike this one, will not
// block / wait for a packet before continuing, that example is
// LoRa > Receive

// The maximum time to wait for a packet is provided in milliseconds,
// 5000 in this case

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
	Feather.begin_serial();
	Feather.parse_config(LCONFIG);
	Feather.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop()
{
	if (Feather.get<Loom::LoRa>()->receive_blocking(5000)) {
		Feather.display_data();
		Feather.get<Loom::SD>()->log("received.csv");
	}
}
