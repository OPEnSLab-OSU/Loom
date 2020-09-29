///////////////////////////////////////////////////////////////////////////////

// This example demonstrates how to put device to sleep and wake via 
// DS3231 RTC alarm connected to pin 6.

// The built-in LED will blink once the device is awake.

// This is not a repeating sleep-wake cycle example.

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In Tools menu, set:
// Internet  > Disabled
// Sensors   > Enabled
// Radios    > Disabled
// Actuators > Disabled
// Max       > Disabled

using namespace Loom;

Loom::Manager Exec{};


// Detach interrupt on wake
void wakeISR() { 
	detachInterrupt(6); 
	LPrintln("Alarm went off"); 
}


void setup() 
{
	Exec.begin_LED();
	digitalWrite(LED_BUILTIN, HIGH);
	// open Serial Monitor to continue, or use Exec.begin_serial();
	// to continue without waiting on user 
	Exec.begin_serial(true);
	Exec.parse_config(LCONFIG);

	// Register ISR to call on wake
	Exec.get<Loom::InterruptManager>().register_ISR(6, wakeISR, LOW, ISR_Type::IMMEDIATE);

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

	// Set an alarm 15 seconds into the future
	Exec.get<Loom::InterruptManager>().RTC_alarm_duration(0, 0, 0, 15);

	// Go to sleep
	LPrintln("Going to sleep");
	Exec.get<Loom::SleepManager>().sleep();

	// This wont be seen unless you close and reopen Serial Monitor
	LPrintln("Awake");
}
