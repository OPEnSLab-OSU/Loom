///////////////////////////////////////////////////////////////////////////////

// This is a basic example of settings a repeating alarm with the DS3231 RTC.

// This program can be used as a base to an RTC interrupt driven program

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In Tools menu, set:
// Internet  > Disabled
// Sensors   > Enabled
// Radios    > Disabled
// Actuators > Disabled
// Max       > Disabled

using namespace Loom;

Loom::Manager Feather{};


#define ALARM_PIN 6

volatile bool alarmFlag = false;
void alarmISR() { 
	detachInterrupt(digitalPinToInterrupt(ALARM_PIN)); 

	Feather.get<Loom::InterruptManager>()->get_RTC_module()->clear_alarms();

	alarmFlag = true;
}


void setup() 
{ 
	Feather.begin_LED();
	Feather.begin_serial(true);
	Feather.parse_config(LCONFIG);
	Feather.print_config();

	Feather.get<Loom::InterruptManager>()->register_ISR(ALARM_PIN, alarmISR, LOW, ISR_Type::IMMEDIATE);
	Feather.get<Loom::InterruptManager>()->RTC_alarm_duration(TimeSpan(10));
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
		// Feather.get<Loom::InterruptManager>()->get_RTC_module()->clear_alarms();
		// post sleep calls this, and in this example it is in the ISR
		
		Feather.get<Loom::InterruptManager>()->reconnect_interrupt(ALARM_PIN); 
		Feather.get<Loom::InterruptManager>()->RTC_alarm_duration(TimeSpan(10)); 

		digitalWrite(LED_BUILTIN, LOW);
		alarmFlag = false;
	}
}
