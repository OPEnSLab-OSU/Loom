///////////////////////////////////////////////////////////////////////////////

// This is a Basic example of RTC DS3231, but able to customize complie Time.

// You will be able to find how in the config.h file

// The params will be [11,true, true, 2020, 10, 15, 16, 20]
// 11 will be TimeZone that can be found in the documentation
// true will be measure local time 
// true will enable to have custom time for UTC
// 2020 will be the new year value
// 10 will be the new month value
// 15 will be the new day value
// 16(3PM) will be the new hour value
// 20 will be the new minute value 

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

// Set enabled modules
LoomFactory<
  Enable::Internet::Disabled,
  Enable::Sensors::Enabled,
  Enable::Radios::Disabled,
  Enable::Actuators::Disabled,
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
  Loom.pause(); 
}