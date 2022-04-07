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

// This is a basic example that demonstrates usage of the Hypnos board
// Deep sleep functionality.

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
// Radios    > Enabled
// Actuators > Enabled
// Max       > Enabled

using namespace Loom;

Loom::Manager Feather{};


const int switchPin = 9;
int switchPos = 0;

volatile bool rtc_flag = false;

void wakeISR_RTC() {
	// disable the interrupt
	detachInterrupt(12);
	rtc_flag = true;
}

void setup()
{
	pinMode(switchPin, INPUT);
	// Needs to be done for Hypno Board
	pinMode(5, OUTPUT);		// Enable control of 3.3V rail
	pinMode(6, OUTPUT);		// Enable control of 5V rail
	pinMode(12, INPUT_PULLUP);		// Enable waiting for RTC interrupt, MUST use a pullup since signal is active low
	pinMode(13, OUTPUT);

	//digitalWrite(switchPin, HIGH);
	//See Above
	digitalWrite(5, LOW);	// Enable 3.3V rail
	digitalWrite(6, HIGH);	// Enable 5V rail
	digitalWrite(13, LOW);

	Feather.begin_serial(true);
	Feather.parse_config(json_config);
	Feather.print_config();

	// Register an interrupt on the RTC alarm pin
	getInterruptManager(Feather).register_ISR(12, wakeISR_RTC, LOW, ISR_Type::IMMEDIATE);

	LPrintln("\n ** Setup Complete ** ");
	Serial.flush();
}


void loop()
{
  //re-enable pin upon wake up
  pinMode(switchPin, INPUT);

  /////////////////////////////////////////////
  //change interval depending the on position of pin
  //interval is the variable that gets passed to TimeSpan function below
  switchPos = digitalRead(switchPin);
  int secs = 0;
  int mins = 0;
  if (switchPos == HIGH) {
    mins = 1200;
  }else{
    secs = 1;
  }
  /////////////////////////////////////////////

	digitalWrite(5, LOW); // Disable 3.3V rail
	digitalWrite(6, HIGH);  // Disable 5V rail
	digitalWrite(13, HIGH);

	// As it turns out, if the SD card is initialized and you change
	// the states of the pins to ANY VALUE, the SD card will fail to
	// write. As a result, we ensure that the board has been turned
	// off at least once before we make any changes to the pin states
	if (rtc_flag) {
		pinMode(23, OUTPUT);
		pinMode(24, OUTPUT);
		pinMode(10, OUTPUT);

		delay(1000);

		Feather.power_up();
	}


	Feather.measure();
	Feather.package();
	Feather.display_data();
	getSD(Feather).log();

	// set the RTC alarm to a duration of 10 seconds with TimeSpan
	getInterruptManager(Feather).RTC_alarm_duration(TimeSpan(0,0,mins,secs));
	getInterruptManager(Feather).reconnect_interrupt(12);

	digitalWrite(13, LOW);
	digitalWrite(5, HIGH); // Enable 3.3V rail
	digitalWrite(6, LOW);  // Enable 5V rail
	pinMode(23, INPUT);
	pinMode(24, INPUT);
	pinMode(10, INPUT);

	// Sleep Manager autmatically calls power_down on every sensor before sleeping
	// And power_up after waking.

	rtc_flag = false;
	getSleepManager(Feather).sleep();
	while (!rtc_flag);
}
