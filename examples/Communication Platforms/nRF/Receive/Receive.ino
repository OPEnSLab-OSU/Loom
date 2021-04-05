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
	Feather.parse_config(json_config);
	Feather.nRF().set_print_verbosity(Verbosity::V_HIGH);
	Feather.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop()
{
	if (Feather.get<Loom::nRF>()->receive()) {
		Feather.display_data();
		Feather.get<Loom::SD>()->log("nrf.csv");
	}

	Feather.pause();	// Pause according to 'interval' in config
					// This controls the frequency of checking
					// for incoming data

}
