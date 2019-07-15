// Example to put device to sleep and wake via 
// DS3231 RTC alarm connected to pin 6

#include <Loom.h>


// Include configuration
const char* json_config = 
#include "config.h"
;

// Detach interrupt on wake
void wakeISR() { 
	detachInterrupt(6); 
	LPrintln("Alarm went off"); 
}

LoomManager Loom("");


void setup() 
{
	Loom.begin_LED();
	digitalWrite(LED_BUILTIN, HIGH);
	// open Serial Monitor to continue, or use Loom.begin_serial();
	// to continue without waiting on user 
	Loom.begin_serial(true);
	Loom.parse_config(json_config);

	// Register ISR to call on wake
	Loom.InterruptManager().register_ISR(6, wakeISR, LOW, ISR_Type::IMMEDIATE);
	
	// Set an alarm 15 seconds into the future
	Loom.InterruptManager().RTC_alarm_duration(0, 0, 0, 15);

	// Go to sleep
	LPrintln("Going to sleep");
	Loom.SleepManager().sleep();
	// LowPower.standby();
	digitalWrite(LED_BUILTIN, LOW);

	LPrintln("\n ** Setup Complete ** ");
}


void loop() 
{
	digitalWrite(LED_BUILTIN, HIGH);
	delay(500);
	digitalWrite(LED_BUILTIN, LOW);
	delay(500);

	// This wont be seen unless you close and reopen Serial Monitor
	LPrintln("Awake");
}
