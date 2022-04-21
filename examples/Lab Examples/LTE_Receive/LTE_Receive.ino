///////////////////////////////////////////////////////////////////////////////

// This is an example of receiving data via LoRa and publishing to Google Sheets
// This example uses the LTE internet platform to connect to Google Sheets

// The corresponding example is LoRa > Transmit

// There is a similar receiving example that, unlike this one, will not
// block / wait for a packet before continuing, that example is
// LoRa > Receive

// The maximum time to wait for a packet is provided in milliseconds,
// 5000 in this case

// Configuration info:
// https://openslab-osu.github.io/Loom/doxygenV2/html/class_loom___l_t_e.html
// https://openslab-osu.github.io/Loom/doxygenV2/html/class_loom___lo_ra.html

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config =
#include "config.h"
;

// In Tools menu, set:
// Internet  > LTE
// Sensors   > Disabled
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
	if (getLoRa(Feather).receive_blocking(5000)) {
		Feather.display_data();
		getGoogleSheets(Feather).publish();
	}
}
