///////////////////////////////////////////////////////////////////////////////

// The only difference between this example an 'Basic' is the LoomFactory
// settings, the line:
//		Feather.GoogleSheets().publish();
// and the configuration, enabling logging to Google Sheets.

// In the config, you need:
// - Network provider name (APN), the GPRS username and password if needed, and the analog pin connected to Pin 5 on the SparkFun LTE shield to avoid manually turning the shield on.
// - For Google sheets parameters, see:
//   https://github.com/OPEnSLab-OSU/Loom/wiki/Using-Loom-with-Google-Sheets

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In Tools menu, set:
// Internet  > LTE
// Sensors   > Enabled
// Radios    > Disabled
// Actuators > Enabled
// Max       > Disabled

using namespace Loom;

Loom::Manager Feather{};


void setup() {
  Feather.begin_serial(true);
  Feather.parse_config(LCONFIG);
  Feather.print_config();

  LPrintln("\n ** Setup Complete ** ");
}

void loop() {
  Feather.measure();
  Feather.package();
  Feather.display_data();

  Feather.get<Loom::GoogleSheets>()->publish();

  Feather.pause();
}
