///////////////////////////////////////////////////////////////////////////////

// This is a basic example of settings a repeating alarm with the DS3231 RTC.

// This program can be used as a base to an RTC interrupt driven program

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



#define ALARM_PIN 6

volatile bool alarmFlag = false;
void alarmISR() { 
	detachInterrupt(digitalPinToInterrupt(ALARM_PIN)); 

	Loom.InterruptManager().get_RTC_module()->clear_alarms();

	alarmFlag = true;
}


void setup() 
{ 
	Loom.begin_LED();
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();

	Loom.InterruptManager().register_ISR(ALARM_PIN, alarmISR, LOW, ISR_Type::IMMEDIATE);
	Loom.InterruptManager().RTC_alarm_duration(TimeSpan(10));
	digitalWrite(LED_BUILTIN, LOW);

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	if (alarmFlag) {
		digitalWrite(LED_BUILTIN, HIGH);
		LPrintln("Alarm triggered, resetting alarm");
		Loom.pause(1000);
		
		// Don't call RTC_alarm_duration before reconnect_interrupt 
		// unless sleeping or calling:
		// Loom.InterruptManager().get_RTC_module()->clear_alarms();
		// post sleep calls this, and in this example it is in the ISR
		
		Loom.InterruptManager().reconnect_interrupt(ALARM_PIN); 
		Loom.InterruptManager().RTC_alarm_duration(TimeSpan(10)); 

		digitalWrite(LED_BUILTIN, LOW);
		alarmFlag = false;
	}
}
