///////////////////////////////////////////////////////////////////////////////

// This example expands upon GoogleSheetsEthernet, so if you haven't looked at
// that one, start there.

// This example add RTC synchronization to time obtained from the internet
// using the NTP_Sync module
// (Google Sheets logging is not necessary for this synchronization)

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In Tools menu, set:
// Internet  > All
// Sensors   > Enabled
// Radios    > Disabled
// Actuators > Enabled
// Max       > Disabled


using namespace Loom;

Loom::Manager Exec{};


void setup()
{
	Exec.begin_serial(true);
	Exec.parse_config(LCONFIG);
	Exec.print_config();

	LPrintln("\n ** Setup Complete ** ");
}


void loop()
{
	Exec.measure();
	Exec.package();
	Exec.display_data();

	Exec.get<Loom::GoogleSheets>().publish();

	Exec.pause();
}
