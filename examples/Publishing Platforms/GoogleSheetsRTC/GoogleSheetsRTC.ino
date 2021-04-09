///////////////////////////////////////////////////////////////////////////////

// This example expands upon GoogleSheetsEthernet, so if you haven't looked at
// that one, start there.

// This example add RTC synchronization to time obtained from the internet
// using the NTP_Sync module
// (Google Sheets logging is not necessary for this synchronization)

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config =
#include "config.h"
;

// In Tools menu, set:
// Internet  > Ethernet
// Sensors   > Enabled
// Radios    > Disabled
// Actuators > Enabled
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

	getGoogleSheets(Feather).publish();

	Feather.pause();
}
