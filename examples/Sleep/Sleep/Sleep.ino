///////////////////////////////////////////////////////////////////////////////

// This example demonstrates how to put device to sleep and wake via
// DS3231 RTC alarm connected to pin 12.

// The built-in LED will blink once the device is awake.

// This is not a repeating sleep-wake cycle example.

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


// Detach interrupt on wake
void wakeISR() {
	detachInterrupt(12);
	LPrintln("Alarm went off");
}


void setup()
{
	// Needs to be done for Hypnos Board
	pinMode(5, OUTPUT);		// Enable control of 3.3V rail
	pinMode(6, OUTPUT);		// Enable control of 5V rail
	pinMode(12, INPUT_PULLUP);		// Enable waiting for RTC interrupt, MUST use a pullup since signal is active low
	pinMode(13, OUTPUT);

	Feather.begin_LED();
	digitalWrite(LED_BUILTIN, HIGH);
	// open Serial Monitor to continue, or use Feather.begin_serial();
	// to continue without waiting on user
	Feather.begin_serial(true);
	Feather.parse_config(json_config);

	// Register ISR to call on wake
	getInterruptManager(Feather).register_ISR(12, wakeISR, LOW, ISR_Type::IMMEDIATE);

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
	getInterruptManager(Feather).RTC_alarm_duration(0, 0, 0, 15);

	// Go to sleep
	LPrintln("Going to sleep");
	Feather.power_down();
	getSleepManager(Feather).sleep();
	Feather.power_up();

	// This wont be seen unless you close and reopen Serial Monitor
	LPrintln("Awake");
}
