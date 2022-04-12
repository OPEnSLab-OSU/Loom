///////////////////////////// IMPORTANT ////////////////////////////////////////////////////////////////////////////////////////////////////
// This example uses a config.h that allows you to set an initial custom time for the RTC (usually your local time).                      //
// In the config file, you will notice this component:                                                                                    //
//                                                                                                                                        //
// {\                                                                                                                                     //
//     'name':'DS3231',\                                                                                                                  //
//     'params':[11,true,true]\                                                                                                           //
// },\                                                                                                                                    //
//                                                                                                                                        //
// The first parameter is the timezone, a list of which can be found on the wiki at the link below.                                       //
// The second parameter allows local time to be displayed alongside UTC time if set to true.                                              //
// The third parameter allows you to enter a custom time using the serial monitor when the RTC is reset if set to true.                   //
//                                                                                                                                        //
// Upon sketch upload, click the magnifying glass in the top-right corner of the IDE to open the serial monitor.                          //
// You will then be prompted to enter a year, month, day, hour, minute, and second.                                                       //
// You can enter this information via an input field at the top of the serial monitor and hitting Enter or clicking Send.                 //
// The time will be converted to UTC for display in the serial monitor, but the custom time will also appear in your log file on the SD.  //
// If you are not prompted to enter a custom time, the RTC should be reset if it does not match the time you'd like.                      //
// Visit the Loom wiki for RTC reset instructions: https://github.com/OPEnSLab-OSU/Loom/wiki/Custom-RTC-Time                              //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This is a basic example that demonstrates usage of the Hypnos board.

// The Hypnos board includes
// - SD
// - DS3231 RTC
// - Ability to power of peripherals

// Further details about the Hypnos board can be found here:
// https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/wiki/Hypnos

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

	// Log using default filename as provided in configuration
	// in this case, 'datafile.csv'
	getSD(Feather).log();

	Feather.pause();
}
