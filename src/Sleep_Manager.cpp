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
  LMark;
	pinMode(power_off_pin, OUTPUT);
}

///////////////////////////////////////////////////////////////////////////////
Loom_Sleep_Manager::Loom_Sleep_Manager(LoomManager* manager, JsonArrayConst p)
	: Loom_Sleep_Manager(manager, p[0], p[1], (Mode)(int)p[2], p[3]) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::print_config() const
{
  LMark;
	LoomModule::print_config();
  LMark;
	LPrintln("\tSleep Mode    : ", enum_sleep_mode_string(sleep_mode) );
  LMark;
	LPrintln("\tUse LED       : ", (use_LED) ? "Enabled" : "Disabled" );
  LMark;
	LPrintln("\tDelay on Wake : ", (delay_on_wake) ? "Enabled" : "Disabled" );
  LMark;
	LPrintln("\t\t Interrupt? ", interrupt_manager ? "+" : "-");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::link_device_manager(LoomManager* LM)
{
  LMark;
	LoomModule::link_device_manager(LM);
  LMark;

	if ( LM ){
  	LMark;

		// Set manager's sleep manager
		LM->sleep_manager = this;
  	LMark;

		// Link to interrupt manager
		auto interrupt_manager = LM->get_interrupt_manager();
   	LMark;
		if ( interrupt_manager ) {
    	LMark;
			link_interrupt_manager(interrupt_manager);
    	LMark;
			interrupt_manager->link_sleep_manager(this);
   		LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Sleep_Manager::sleep()
{
   	LMark;
		pre_sleep();
  	LMark;

		LowPower.standby();
   	LMark;
		// This is where programs waits until waking

		post_sleep();
 		LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::pre_sleep()
{
  LMark;
	LPrintln("\nEntering STANDBY");
  LMark;
	Serial.flush();
  LMark;
	Serial.end();
  LMark;
	USBDevice.detach();
  LMark;

	// Disable SysTick Interrupt
	// See https://community.atmel.com/comment/2616121#comment-2616121
	// calls to delay() and Serial.xx() will not work after this line
	SysTick->CTRL = 0;
  LMark;

	digitalWrite(LED_BUILTIN, LOW);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::post_sleep()
{
  LMark;
	// Reenable SysTick Interrupt
	// See https://community.atmel.com/comment/2616121#comment-2616121
	// calls to delay() and Serial.xx() will begin working after this line
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk
				| SysTick_CTRL_TICKINT_Msk
				| SysTick_CTRL_ENABLE_Msk;
  LMark;

	if (use_LED) {
   	LMark;
		digitalWrite(LED_BUILTIN, HIGH);
  	LMark;
	}

}

///////////////////////////////////////////////////////////////////////////////
const char* Loom_Sleep_Manager::enum_sleep_mode_string(const Mode m)
{
  LMark;
	switch(m) {
   	LMark;
		case Mode::IDLE				: return "Idle";
   	LMark;
		case Mode::STANDBY			: return "Standby";
   	LMark;
		case Mode::OPENS_LOWPOWER	: return "OPEnS_Lowpower";
   	LMark;
		default : return "";
	}
}

///////////////////////////////////////////////////////////////////////////////
