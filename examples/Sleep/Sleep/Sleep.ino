///////////////////////////// IMPORTANT ////////////////////////////////////////////////////////////////////////////////////////////////////
// This example uses a config.h that allows you to set an initial custom time for the RTC (usually your local time).                      //
// Upon sketch upload, click the magnifying glass in the top-right corner of the IDE to open the serial monitor.                          //
// You will then be prompted to enter a year, month, day, hour, minute, and second.                                                       //
// You can enter this information via an input field at the top of the serial monitor and hitting Enter or clicking Send.                 //
// The time will be converted to UTC for display in the serial monitor, but the custom time will also appear in your log file on the SD.  //
// If you are not prompted to enter a custom time, the RTC should be reset if it does not match the time you'd like.                      //
// Visit the Loom wiki for RTC reset instructions: https://github.com/OPEnSLab-OSU/Loom/wiki/Setting-Custom-RTC-Time                      //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
