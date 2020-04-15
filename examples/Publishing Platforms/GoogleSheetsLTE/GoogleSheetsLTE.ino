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

  Loom.GoogleSheets().publish();

  Loom.pause();
}
