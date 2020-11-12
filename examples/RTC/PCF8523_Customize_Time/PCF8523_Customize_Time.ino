///////////////////////////////////////////////////////////////////////////////

// This is a Basic example of RTC PCF8523, but able to customize Time.

// The params will be [11, true, true]
// 11 will be TimeZone that can be found in the documentation
// true will be measure local time 
// true will enable to have custom time for Local time input

// You need to enter the year, month, day hour, minute, and seconds to get 
// the most accurate time in the serial monitor. 
// Note that hour is up to 23. In other words, 3PM is 15 not 3. 3 will be 3 AM.

// Documentation for PCF8523 RTC: https://openslab-osu.github.io/Loom/doxygenV2/html/class_loom___p_c_f8523.html

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