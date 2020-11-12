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

// Set enabled modules
LoomFactory<
	Enable::Internet::LTE,
	Enable::Sensors::Disabled,
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
	if (Loom.LoRa().receive_blocking(5000)) {
		Loom.display_data();
		Loom.GoogleSheets().publish();
	}
}
