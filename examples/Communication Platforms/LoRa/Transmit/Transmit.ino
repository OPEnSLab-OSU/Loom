///////////////////////////////////////////////////////////////////////////////

// This is a basic example of sending data via LoRa.

// The corresponding example is LoRa > Receive (or Receive_Blocking)

// These two examples are the Loom equivalent of the basic RX / TX LoRa
// examples

// See https://openslab-osu.github.io/Loom/html/class_loom___lo_ra.html
// for details of LoRa config options

// While you can send whatever data you collect, for the purposes of this
// example, only analog data is being collected

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
	Feather.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop()
{
	Feather.measure();
	Feather.package();
	Feather.display_data();

	// Send to address 1
	getLoRa(Feather).send(1);

	Feather.pause();	// Delay between interations set with 'interval' in config
}
