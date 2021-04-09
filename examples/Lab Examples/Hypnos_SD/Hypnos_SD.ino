///////////////////////////////////////////////////////////////////////////////

// This is a basic example that demonstrates usage of the Hypnos board.

// The Hypnos board includes
// - SD
// - DS3231 RTC
// - Ability to power of peripherals

// Further details about the Hypnos board can be found here:
// https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/wiki/Hypnos

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
// Actuators > Enabled
// Max       > Enabled

using namespace Loom;

Loom::Manager Feather{};


void setup()
{
	// Needs to be done for Hypno Board
	pinMode(5, OUTPUT);		// Enable control of 3.3V rail
	pinMode(6, OUTPUT);		// Enable control of 5V rail

	//See Above
	digitalWrite(5, LOW);	// Enable 3.3V rail
	digitalWrite(6, HIGH);	// Enable 5V rail

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

	// Log using default filename as provided in configuration
	// in this case, 'datafile.csv'
	getSD(Feather).log();

	Feather.pause();
}
