///////////////////////////////////////////////////////////////////////////////

// This is a basic example that demonstrates usage of the Hypnos board
// Deep sleep functionality.

// The Hypnos board includes
// - SD
// - DS3231 RTC
// - Ability to power of peripherals

// Further details about the Hypnos board can be found here:
// https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/wiki/Hypnos

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

#define RTC_INT_PIN 12
#define HYPNOS3 5
#define HYPNOS5 6

// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Enabled,
	Enable::Radios::Enabled,
	Enable::Actuators::Enabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

volatile bool rtc_flag = false;

void wakeISR_RTC() {
	// disable the interrupt
	detachInterrupt(RTC_INT_PIN);
	rtc_flag = true;
}

void setup() 
{
	// Needs to be done for Hypno Board
	pinMode(HYPNOS3, OUTPUT);		// Enable control of 3.3V rail 
	pinMode(HYPNOS5, OUTPUT);		// Enable control of 5V rail 
	pinMode(RTC_INT_PIN, INPUT_PULLUP);		// Enable waiting for RTC interrupt, MUST use a pullup since signal is active low
    pinMode(13, OUTPUT);

	//See Above
	digitalWrite(HYPNOS3, LOW);	// Enable 3.3V rail
	digitalWrite(HYPNOS5, HIGH);	// Enable 5V rail
    digitalWrite(13, LOW);

	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();

	// Register an interrupt on the RTC alarm pin
	Loom.InterruptManager().register_ISR(RTC_INT_PIN, wakeISR_RTC, LOW, ISR_Type::IMMEDIATE);

	LPrintln("\n ** Setup Complete ** ");
	Serial.flush();
}


void loop() 
{
	digitalWrite(HYPNOS3, LOW); // Enable 3.3V rail
  digitalWrite(HYPNOS5, HIGH);  // Enable 5V rail
  digitalWrite(13, HIGH);

	// As it turns out, if the SD card is initialized and you change
	// the states of the pins to ANY VALUE, the SD card will fail to
	// write. As a result, we ensure that the board has been turned
	// off at least once before we make any changes to the pin states
	if (rtc_flag) {
		pinMode(23, OUTPUT);
		pinMode(24, OUTPUT);
		pinMode(10, OUTPUT);

    // delay(1000);

		Loom.power_up();
	}

	Loom.measure();
	Loom.package();
	Loom.display_data();

	Loom.SDCARD().log();

	// set the RTC alarm to a duration of 10 seconds with TimeSpan
	Loom.InterruptManager().RTC_alarm_duration(TimeSpan(0,0,0,10));
	Loom.InterruptManager().reconnect_interrupt(RTC_INT_PIN);

    digitalWrite(13, LOW);
	digitalWrite(HYPNOS3, HIGH); // Disable 3.3V rail
	digitalWrite(HYPNOS5, LOW);  // Disable 5V rail
	pinMode(23, INPUT);
	pinMode(24, INPUT);
	pinMode(10, INPUT);

	// Sleep Manager autmatically calls power_down on every sensor before sleeping
	// And power_up after waking.

    rtc_flag = false;
	Loom.SleepManager().sleep();
	while (!rtc_flag);
}