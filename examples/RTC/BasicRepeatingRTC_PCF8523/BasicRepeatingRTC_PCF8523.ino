///////////////////////////// IMPORTANT ////////////////////////////////////////////////////////////////////////////////////////////////////
// This example uses a config.h that allows you to set an initial custom time for the RTC (usually your local time).                      //
// In the config file, you will notice this component:                                                                                    //
//                                                                                                                                        //
// {\                                                                                                                                     //
//     'name':'PCF8523',\                                                                                                                 //
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

// This is a basic example of settings a repeating alarm with the PCF8523 RTC.

// This program can be used as a base to an RTC interrupt driven program

// Note that the PCF8523 is different than the DS3231 in a few aspects
// - Alarms only have minute (rather than second) precision
//		Any alarms set for the current minute will be delayed until the
//		start of the next minute
// - The interrupt is a constant low signal, rather than a squareware
//		No need to detach the interrupts, just clear the alarms

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


#define ALARM_PIN 12

volatile bool alarmFlag = false;
volatile int count = 0;
void alarmISR() {
	getInterruptManager(Feather).get_RTC_module()->clear_alarms();
	count++;
	alarmFlag = true;
}


void setup()
{
	Feather.begin_LED();
	Feather.begin_serial(true);
	Feather.parse_config(json_config);
	Feather.print_config();

	getInterruptManager(Feather).register_ISR(ALARM_PIN, alarmISR, LOW, ISR_Type::IMMEDIATE);
	getInterruptManager(Feather).RTC_alarm_duration(TimeSpan(10));
	digitalWrite(LED_BUILTIN, LOW);

	LPrintln("\n ** Setup Complete ** ");
}

void loop()
{
	if (alarmFlag) {
		digitalWrite(LED_BUILTIN, HIGH);
		LPrintln("Alarm triggered, resetting alarm");
		Feather.pause(1000);

		getInterruptManager(Feather).RTC_alarm_duration(TimeSpan(10));

		digitalWrite(LED_BUILTIN, LOW);
		alarmFlag = false;
	}

	LPrintln("Count: ", count);
	getPCF8523(Feather).print_time();
	Feather.pause(1000);

}
