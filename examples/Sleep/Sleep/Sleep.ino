// Example to put device to sleep and wake via 
// DS3231 RTC alarm connected to pin 6

#include <Loom.h>

// #include "LowPower.h"


// Include configuration
const char* json_config = 
#include "config.h"
;

// Detach interrupt on wake
void wakeISR() { 
	LPrintln("Alarm went off"); 
	detachInterrupt(6); 
}

LoomManager Manager("Manager", "Loom", 1, 1, LoomManager::DeviceType::NODE, Verbosity::V_LOW, Verbosity::V_LOW);


void setup() 
{
	pinMode(LED_BUILTIN, OUTPUT);   	
	digitalWrite(LED_BUILTIN, HIGH);

	Serial.begin(115200);

	// Wait for Serial Monitor to continue
	// Comment out the following line to run without Serial
	while(!Serial);       		
	delay(1000);

	LPrintln("Initialized Serial!\n");

	Manager.parse_config(json_config);

	// Register ISR to call on wake
	Manager.InterruptManager().register_ISR(6, wakeISR, LOW, ISR_Type::IMMEDIATE);
	
	// Set an alarm 15 seconds into the future
	Manager.InterruptManager().RTC_alarm_duration(0, 0, 0, 15);

	// Go to sleep
	LPrintln("Going to sleep");
	Manager.SleepManager().sleep();
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
