///////////////////////////////////////////////////////////////////////////////

// The only difference between this example an 'Basic' is the LoomFactory
// settings, the line:
//		Loom.Spool().publish();
// and the configuration, enabling logging to Spool.

// Spool is data logging and device management platform for Loom

// In the config, you need:
// - Network provider name (APN), the GPRS username and password if needed, 
//      and the analog pin connected to Pin 5 on the SparkFun LTE shield 
//      to avoid manually turning the shield on.

// - For Spool parameters, see:
//   https://github.com/OPEnSLab-OSU/Loom/wiki/Using-Loom-with-Spool
//   https://github.com/OPEnSLab-OSU/Spool/wiki

///////////////////////////////////////////////////////////////////////////////


#include <Loom.h>

const char* json_config =
#include "config.h"
;

LoomFactory<
  Enable::Internet::LTE,
  Enable::Sensors::Enabled,
  Enable::Radios::Disabled,
  Enable::Actuators::Enabled,
  Enable::Max::Disabled
> ModuleFactory{};


LoomManager Loom{ &ModuleFactory };


void setup() {
  Loom.begin_serial(true);
  Loom.parse_config(json_config);
  Loom.print_config();

  LPrintln("\n ** Setup Complete ** ");
}

void loop() {
  Loom.measure();
  Loom.package();
  Loom.display_data();

  Loom.Spool().publish();

  Loom.pause();
}