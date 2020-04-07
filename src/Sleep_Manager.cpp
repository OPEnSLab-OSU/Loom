///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Sleep_Manager.cpp
/// @brief		File for Loom_Sleep_Manager implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Sleep_Manager.h"
#include "RTC/RTC.h"

#include <Adafruit_SleepyDog.h>
#include <LowPower.h>


///////////////////////////////////////////////////////////////////////////////
Loom_Sleep_Manager::Loom_Sleep_Manager( 
		LoomManager* 	manager,
		const bool					use_LED, 
		const bool					delay_on_wake, 
		const Mode					sleep_mode,
		const byte					power_off_pin
	) 
	: LoomModule(manager, "SleepManager", Type::Sleep_Manager )
	, use_LED(use_LED)
	, delay_on_wake(delay_on_wake)
	, sleep_mode(Mode::STANDBY)
	, power_off_pin(power_off_pin)
{
	pinMode(power_off_pin, OUTPUT);
}

///////////////////////////////////////////////////////////////////////////////
Loom_Sleep_Manager::Loom_Sleep_Manager(LoomManager* manager, JsonArrayConst p)
	: Loom_Sleep_Manager(manager, p[0], p[1], (Mode)(int)p[2], p[3]) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::print_config() const
{
	LoomModule::print_config();
	LPrintln("\tSleep Mode    : ", enum_sleep_mode_string(sleep_mode) );
	LPrintln("\tUse LED       : ", (use_LED) ? "Enabled" : "Disabled" );
	LPrintln("\tDelay on Wake : ", (delay_on_wake) ? "Enabled" : "Disabled" );
	LPrintln("\t\t Interrupt? ", interrupt_manager ? "+" : "-");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::link_device_manager(LoomManager* LM)
{
	LoomModule::link_device_manager(LM);

	if ( LM ){

		// Set manager's sleep manager 
		LM->sleep_manager = this;

		// Link to interrupt manager
		auto interrupt_manager = LM->get_interrupt_manager(); 
		if ( interrupt_manager ) {
			link_interrupt_manager(interrupt_manager);
			interrupt_manager->link_sleep_manager(this);
		}
	}
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
	LPrintln("\nEntering STANDBY");
	Serial.flush();
	Serial.end();
	USBDevice.detach();

	// Disable SysTick Interrupt
	// See https://community.atmel.com/comment/2616121#comment-2616121
	// calls to delay() and Serial.xx() will not work after this line
	SysTick->CTRL = 0;

	digitalWrite(LED_BUILTIN, LOW);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::post_sleep()
{
	// Reenable SysTick Interrupt
	// See https://community.atmel.com/comment/2616121#comment-2616121
	// calls to delay() and Serial.xx() will begin working after this line
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk
				| SysTick_CTRL_TICKINT_Msk
				| SysTick_CTRL_ENABLE_Msk;

	if (use_LED) {
		digitalWrite(LED_BUILTIN, HIGH);
	}

}

///////////////////////////////////////////////////////////////////////////////
const char* Loom_Sleep_Manager::enum_sleep_mode_string(const Mode m)
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

