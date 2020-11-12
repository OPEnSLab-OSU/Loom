///////////////////////////////////////////////////////////////////////////////

// This example expands upon GoogleSheetsEthernet, so if you haven't looked at
// that one, start there.

// This example add RTC synchronization to time obtained from the internet
// using the NTP_Sync module
// (Google Sheets logging is not necessary for this synchronization)

// Documentation for GoogleSheets: https://openslab-osu.github.io/Loom/doxygenV2/html/class_loom___google_sheets.html
// Documentation for DS3231 RTC: https://openslab-osu.github.io/Loom/doxygenV2/html/class_loom___d_s3231.html

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config =
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::All,
	Enable::Sensors::Enabled,
	Enable::Radios::Disabled,
	Enable::Actuators::Enabled,
	Enable::Max::Disabled
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
