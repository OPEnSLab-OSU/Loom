///////////////////////////// IMPORTANT ////////////////////////////////////////////////////////////////////////////////////////////////////
// This example uses a config.h that allows you to set an initial custom time for the RTC (usually your local time).                      //
// Upon sketch upload, click the magnifying glass in the top-right corner of the IDE to open the serial monitor.                          //
// You will then be prompted to enter a year, month, day, hour, minute, and second.                                                       //
// You can enter this information via an input field at the top of the serial monitor and hitting Enter or clicking Send.                 //
// The time will be converted to UTC for display in the serial monitor, but the custom time will also appear in your log file on the SD.  //
// If you are not prompted to enter a custom time, the RTC should be reset if it does not match the time you'd like.                      //
// Visit the Loom wiki for RTC reset instructions: https://github.com/OPEnSLab-OSU/Loom/wiki/Setting-Custom-RTC-Time                      //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// This example is the code used for the OPEnS Lab's Smart Rock

// It is using the following modules:
// - DS3231 RTC
// - Interrupt Manager
// - Sleep Manager
// - Analog (for turbitidy, electrical conductivity, pH, total dissolved solids)
// - SD
// - MS5803 pressure sensor

// Normal operation is:
// - Collect data
// - Log data to SD card
// - Set alarm for next wake time
// - Go to sleep until alarm

// You can also wake the Smart Rock by putting a magnet near the reed switch.
// This will also trigger data logging and then return to sleep

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config =
#include "config.h"
;

// In Tools menu, set:
// Internet  > Disabled
// Sensors   > Enabled
// Radios    > Enabled
// Actuators > Enabled
// Max       > Enabled

using namespace Loom;

Loom::Manager Feather{};


#define ALARM_PIN 6		// Wire interrupt on RTC to this pin
#define REED_PIN 11		// Wire reed switch to this pin


volatile bool alarmFlag = false;
volatile bool reedFlag = false;


void alarmISR() {
	detachInterrupt(digitalPinToInterrupt(ALARM_PIN));
	// detachInterrupt(digitalPinToInterrupt(REED_PIN));
	alarmFlag = true;
	// LPrintln("\n\n", "Alarm went off", "\n");
}

void reedISR() {
	// detachInterrupt(digitalPinToInterrupt(ALARM_PIN));
	detachInterrupt(digitalPinToInterrupt(REED_PIN));
	reedFlag = true;
	// LPrintln("\n\n", "Reed switch triggered", "\n");
}



void setup()
{
	Feather.begin_LED();
	Feather.flash_LED(10, 200, 200, true);
	Feather.begin_serial();

	pinMode(10, OUTPUT);

	Feather.parse_config(json_config);
	Feather.print_config();

	// pinMode(ALARM_PIN, INPUT_PULLUP);
	// pinMode(REED_PIN, INPUT_PULLUP);

	getInterruptManager(Feather).register_ISR(ALARM_PIN, alarmISR, LOW, ISR_Type::IMMEDIATE);
	getInterruptManager(Feather).register_ISR(REED_PIN, reedISR, LOW, ISR_Type::IMMEDIATE);

	LPrintln("\n ** Setup Complete ** ");
}


void loop()
{
	LPrintln("AlarmFlag : ", alarmFlag);
	LPrintln("ReedFlag  : ", reedFlag);

	digitalWrite(LED_BUILTIN, HIGH);

	digitalWrite(10, HIGH);
	Feather.pause(200);

	Feather.measure();

	digitalWrite(10, LOW);

	Feather.package();
	Feather.add_data("wakeType", "type", alarmFlag ? "alarm" : "reed");
	Feather.display_data();
	getSD(Feather).log();

	Feather.pause(500);
	getInterruptManager(Feather).register_ISR(ALARM_PIN, alarmISR, LOW, ISR_Type::IMMEDIATE);
	getInterruptManager(Feather).register_ISR(REED_PIN, reedISR, LOW, ISR_Type::IMMEDIATE);
	getInterruptManager(Feather).RTC_alarm_duration(TimeSpan(600));

	// delay(4000);

	alarmFlag = false;
	reedFlag = false;

	// Go to sleep
	getSleepManager(Feather).sleep();
	//

}
