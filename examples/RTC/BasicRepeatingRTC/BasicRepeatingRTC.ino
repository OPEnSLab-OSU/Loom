#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

LoomManager Loom("");


#define ALARM_PIN 6

volatile bool alarmFlag = false;
void alarmISR() { 
	detachInterrupt(digitalPinToInterrupt(ALARM_PIN)); 
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
		delay(1000);
		Loom.InterruptManager().RTC_alarm_duration(TimeSpan(10)); // Has to happen before reconnect_interrupt
		Loom.InterruptManager().reconnect_interrupt(ALARM_PIN); 
		digitalWrite(LED_BUILTIN, LOW);
		alarmFlag = false;
	}
}
