
#include "Loom_Sleep_Manager.h"
#include "RTC/Loom_RTC.h"

#include <Adafruit_SleepyDog.h>
#include <LowPower.h>


///////////////////////////////////////////////////////////////////////////////
Loom_Sleep_Manager::Loom_Sleep_Manager( 
		bool			use_LED, 
		bool 			delay_on_wake, 
		Mode			sleep_mode,
		byte			power_off_pin
	) : LoomModule( "SleepManager", Type::Sleep_Manager )
	, use_LED(use_LED)
	, delay_on_wake(delay_on_wake)
	, sleep_mode(Mode::STANDBY)
	, power_off_pin(power_off_pin)
{
	pinMode(power_off_pin, OUTPUT);
}

///////////////////////////////////////////////////////////////////////////////
Loom_Sleep_Manager::Loom_Sleep_Manager(JsonArrayConst p)
	: Loom_Sleep_Manager(p[0], p[1], (Mode)(int)p[2], p[3]) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::link_interrupt_manager(Loom_Interrupt_Manager* IM)
{
	interrupt_manager = IM;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::print_config()
{
	LoomModule::print_config();
	LPrintln("\tSleep Mode    : ", enum_sleep_mode_string(sleep_mode) );
	LPrintln("\tUse LED       : ", (use_LED) ? "Enabled" : "Disabled" );
	LPrintln("\tDelay on Wake : ", (delay_on_wake) ? "Enabled" : "Disabled" );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::print_state()
{
	LoomModule::print_state();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::powerDown()
{
	// Call manager to coordinate modules saving to flash

// to be implemented

	// Use manager to power all modules off
	if (device_manager != nullptr) {
		device_manager->power_down();
	}

	LPrintln("Powering Off");
	digitalWrite(power_off_pin, HIGH);
	delay(50); // Sometimes the board takes several milliseconds to fully power off
	LPrintln("This should not be printed");
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Sleep_Manager::sleep()
{
		pre_sleep();

		LowPower.standby();
		// This is where programs waits until waking

		post_sleep();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::pre_sleep()
{
	// Use manager to power all modules off
	if (device_manager != nullptr) {
		device_manager->power_down();
	}

	LPrintln("\nEntering STANDBY");
	delay(50);
	Serial.end();
	USBDevice.detach();

	digitalWrite(LED_BUILTIN, LOW);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::post_sleep()
{
	// Prevent double trigger of alarm interrupt
	if (interrupt_manager) {
		interrupt_manager->get_RTC_module()->clear_alarms();
	}

	if (use_LED) {
		digitalWrite(LED_BUILTIN, HIGH);
	}

	// Use manager to power all active modules up
	if (device_manager != nullptr) {
		device_manager->power_up();
	}


	#if LOOM_DEBUG == 1
		USBDevice.attach();
		Serial.begin(115200);
		if (delay_on_wake) {
			// Give user 5s to reopen Serial monitor!
			// Note that the serial may still take a few seconds 
			// to fully setup after the LED turns on
			delay(5000); 
		}
		LPrintln("WAKE");
	#endif

	// Set wake time

}

///////////////////////////////////////////////////////////////////////////////
const char* Loom_Sleep_Manager::enum_sleep_mode_string(Mode m)
{
	switch(m) {
		case Mode::IDLE				: return "Idle";
		case Mode::STANDBY			: return "Standby";
		case Mode::SLEEPYDOG		: return "SleepyDog";
		case Mode::OPENS_LOWPOWER	: return "OPEnS_Lowpower";
		default : return "";
	}
}

///////////////////////////////////////////////////////////////////////////////

