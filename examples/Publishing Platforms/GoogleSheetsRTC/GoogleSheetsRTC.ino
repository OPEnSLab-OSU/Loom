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

// Set enabled modules
LoomFactory<
	Loom_Analog,
	Loom_Digital,
	Loom_GoogleSheets,
	Loom_Ethernet,
	Loom_NTP,
	Loom_DS3231
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };



void setup()
{
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();



	LPrintln("\n ** Setup Complete ** ");
}



void loop()
{
	Loom.measure();
	Loom.package();
	Loom.display_data();

	Loom.GoogleSheets().publish();

	Loom.pause();
}
