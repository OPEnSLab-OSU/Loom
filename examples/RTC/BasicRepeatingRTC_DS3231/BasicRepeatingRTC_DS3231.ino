///////////////////////////// IMPORTANT ////////////////////////////////////////////////////////////////////////////////////////////////////
// This example uses a config.h that allows you to set an initial custom time for the RTC (usually your local time).                      //
// Upon sketch upload, click the magnifying glass in the top-right corner of the IDE to open the serial monitor.                          //
// You will then be prompted to enter a year, month, day, hour, minute, and second.                                                       //
// You can enter this information via an input field at the top of the serial monitor and hitting Enter or clicking Send.                 //
// The time will be converted to UTC for display in the serial monitor, but the custom time will also appear in your log file on the SD.  //
// If you are not prompted to enter a custom time, the RTC should be reset if it does not match the time you'd like.                      //
// Visit the Loom wiki for RTC reset instructions: https://github.com/OPEnSLab-OSU/Loom/wiki/Setting-Custom-RTC-Time                      //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This is a basic example of settings a repeating alarm with the DS3231 RTC.

// This program can be used as a base to an RTC interrupt driven program

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
void alarmISR() {
	detachInterrupt(digitalPinToInterrupt(ALARM_PIN));

	getInterruptManager(Feather).get_RTC_module()->clear_alarms();

	alarmFlag = true;
}


void setup()
{
	// Needs to be done for Hypnos Board
	pinMode(5, OUTPUT);		// Enable control of 3.3V rail
	pinMode(6, OUTPUT);		// Enable control of 5V rail
	digitalWrite(5, LOW);	// Enable 3.3V rail
	digitalWrite(6, HIGH);	// Enable 5V rail

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

		// Don't call RTC_alarm_duration before reconnect_interrupt
		// unless sleeping or calling:
		// getInterruptManager(Feather).get_RTC_module()->clear_alarms();
		// post sleep calls this, and in this example it is in the ISR

		getInterruptManager(Feather).reconnect_interrupt(ALARM_PIN);
		getInterruptManager(Feather).RTC_alarm_duration(TimeSpan(10));

		digitalWrite(LED_BUILTIN, LOW);
		alarmFlag = false;
	}
}
