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

// In Tools menu, set:
// Internet  > Disabled
// Sensors   > Enabled
// Radios    > Enabled
// Actuators > Enabled
// Max       > Enabled

using namespace Loom;

Loom::Manager Exec{};

volatile bool rtc_flag = false;

void wakeISR_RTC() {
	// disable the interrupt
	detachInterrupt(12);
	rtc_flag = true;
}

void setup() 
{
	// Needs to be done for Hypno Board
	pinMode(5, OUTPUT);		// Enable control of 3.3V rail 
	pinMode(6, OUTPUT);		// Enable control of 5V rail 
	pinMode(12, INPUT_PULLUP);		// Enable waiting for RTC interrupt, MUST use a pullup since signal is active low
	pinMode(13, OUTPUT);

	//See Above
	digitalWrite(5, LOW);	// Enable 3.3V rail
	digitalWrite(6, HIGH);	// Enable 5V rail
	digitalWrite(13, LOW);

	Exec.begin_serial(true);
	Exec.parse_config(LCONFIG);
	Exec.print_config();

	// Register an interrupt on the RTC alarm pin
	Exec.get<Loom::InterruptManager>().register_ISR(12, wakeISR_RTC, LOW, ISR_Type::IMMEDIATE);

	LPrintln("\n ** Setup Complete ** ");
	Serial.flush();
}


void loop() 
{
	digitalWrite(5, LOW); // Disable 3.3V rail
	digitalWrite(6, HIGH);  // Disable 5V rail
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

		Exec.power_up();
	}

	Exec.measure();
	Exec.package();
	Exec.display_data();

	Exec.get<Loom::SD>().log();

	// set the RTC alarm to a duration of 10 seconds with TimeSpan
	Exec.get<Loom::InterruptManager>().RTC_alarm_duration(TimeSpan(0,0,0,10));
	Exec.get<Loom::InterruptManager>().reconnect_interrupt(12);

	digitalWrite(13, LOW);
	digitalWrite(5, HIGH); // Enable 3.3V rail
	digitalWrite(6, LOW);  // Enable 5V rail
	pinMode(23, INPUT);
	pinMode(24, INPUT);
	pinMode(10, INPUT);

	// Sleep Manager autmatically calls power_down on every sensor before sleeping
	// And power_up after waking.

	rtc_flag = false;
	Exec.get<Loom::SleepManager>().sleep();
	while (!rtc_flag);
}