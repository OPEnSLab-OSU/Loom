///////////////////////////////////////////////////////////////////////////////

// This example demonstrates how to to receive a configuration from the user
// via Serial

// The primary difference between this and the 'Basic' example is the
// 'Feather.parse_config_serial()' line rather than the 'Feather.parse_config(json_config)'

// Upon startup, the device will wait for the user to send a configuration over
// Serial. The LED will blink a few times when ready to receive.

// The LED will blink quickly upon receiving and parsing a valid configuration.

// Additionally, this sketch allows you to send a new configuration during operation
// to re-configure the device using the: 'Feather.check_serial_for_config();' line


// Sending the configuration over Serial can be be done in a variety of ways,
// and may look slightly different depending on operating system.

// If you configuration is already in a text file as a JSON, you can use a command
// from the command line that may look like:
// > cat config.txt > /dev/cu.usbmodem14101

// If you configuration is in a .json file you can use:
// > cat config.json > /dev/cu.usbmodem14101

// Or, to send a smaller JSON string from a .json file with jq:
// > cat JsonTest.json | jq --compact-output '.' | tr -d '\n' > /dev/cu.usbmodem14101

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In Tools menu, set:
// Internet  > All
// Sensors   > Enabled
// Radios    > Disabled
// Actuators > Disabled
// Max       > Disabled

using namespace Loom;

Loom::Manager Feather{};


void setup()
{
	Feather.begin_LED();
	Feather.begin_serial(false);

	// Feather.parse_config(LCONFIG);
	Feather.parse_config_serial();	// Accept config via Serial

	Feather.print_config();

	LPrintln("\n ** Setup Complete ** ");
}



void loop()
{
	Feather.check_serial_for_config();	// Check if a new config has been sent via Serial

	Feather.measure();
	Feather.package();
	Feather.display_data();

	Feather.pause();
}
