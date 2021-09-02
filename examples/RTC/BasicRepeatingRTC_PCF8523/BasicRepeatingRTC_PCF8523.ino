///////////////////////////////////////////////////////////////////////////////

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
