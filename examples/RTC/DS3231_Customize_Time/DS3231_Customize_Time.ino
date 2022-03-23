///////////////////////////// IMPORTANT ////////////////////////////////////////////////////////////////////////////////////////////////////
// This example uses a config.h that allows you to set an initial custom time for the RTC (usually your local time).                      //
// Upon sketch upload, click the magnifying glass in the top-right corner of the IDE to open the serial monitor.                          //
// You will then be prompted to enter a year, month, day, hour, minute, and second.                                                       //
// You can enter this information via an input field at the top of the serial monitor and hitting Enter or clicking Send.                 //
// The time will be converted to UTC for display in the serial monitor, but the custom time will also appear in your log file on the SD.  //
// If you are not prompted to enter a custom time, the RTC should be reset if it does not match the time you'd like.                      //
// Visit the Loom wiki for RTC reset instructions: https://github.com/OPEnSLab-OSU/Loom/wiki/Setting-Custom-RTC-Time                      //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This is a Basic example of RTC DS3231, but able to customize Time.

// The params will be [11, true, true]
// 11 will be TimeZone that can be found in the documentation
// true will be measure local time
// true will enable to have custom time for Local time input

// You need to enter the year, month, day hour, minute, and seconds to get
// the most accurate time in the serial monitor.
// Note that hour is up to 23. In other words, 3PM is 15 not 3. 3 will be 3 AM.

// Documentation for DS3231 RTC: https://openslab-osu.github.io/Loom/doxygenV2/html/class_loom___d_s3231.html

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config =
#include "config.h"
;

// In Tools menu, set:
// Internet  > Disabled
// Sensors   > Enabled
// Radios    > Disabled
// Actuators > Disabled
// Max       > Disabled

using namespace Loom;

Loom::Manager Feather{};

void setup()
{
  // Needs to be done for Hypnos Board
	pinMode(5, OUTPUT);		// Enable control of 3.3V rail
	pinMode(6, OUTPUT);		// Enable control of 5V rail
	digitalWrite(5, LOW);	// Enable 3.3V rail
	digitalWrite(6, HIGH);	// Enable 5V rail

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
  Feather.pause();
}
