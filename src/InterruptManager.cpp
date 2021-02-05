///////////////////////////////////////////////////////////////////////////////
///
/// @file		InterruptManager.cpp
/// @brief		File for InterruptManager implementation
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "InterruptManager.h"
#include "SleepManager.h"
#include "RTC/RTC.h"
#include "Manager.h"
#include "Module_Factory.h"

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>

using namespace Loom;


///////////////////////////////////////////////////////////////////////////////

bool InterruptManager::interrupt_triggered[InteruptRange] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

///////////////////////////////////////////////////////////////////////////////

const ISRFuncPtr InterruptManager::default_ISRs[InteruptRange] =
{
	default_ISR_0,  default_ISR_1,  default_ISR_2,  default_ISR_3,
	default_ISR_4,  default_ISR_5,  default_ISR_6,  default_ISR_7,
	default_ISR_8,  default_ISR_9,  default_ISR_10, default_ISR_11,
	default_ISR_12, default_ISR_13, default_ISR_14, default_ISR_15
};

///////////////////////////////////////////////////////////////////////////////
const char* InterruptManager::interrupt_type_to_string(const uint8_t type)
{
  LMark;
	switch(type) {
		case 0  : return "LOW";
		case 1  : return "HIGH";
		case 2  : return "CHANGE";
		case 3  : return "FALLING";
		case 4  : return "RISING";
		default : return "Invalid";
	}
}

///////////////////////////////////////////////////////////////////////////////
InterruptManager::InterruptManager(
		RTC*		RTC_Inst
	)
	: Module("InterruptManager")
	, RTC_Inst(RTC_Inst)
{
  LMark;

	interrupts_enabled = true;
  LMark;

	for (auto i = 0; i < InteruptRange; i++) {
		LMark;
		int_settings[i] = {-1, nullptr, 0, ISR_Type::IMMEDIATE, true};

	}
	for (auto i = 0; i < MaxTimerCount; i++) {
    LMark;
		timer_settings[i] = {nullptr, 0, false, false};
	}
	for (auto i = 0; i < MaxStopWatchCount; i++) {
    LMark;
		stopwatch_settings[i] = {0, false};
	}

	// Setup the RTCCounter for internal timer
	rtcCounter.begin();
  LMark;

	// Note initial wake time
	if (this->RTC_Inst != nullptr) {
    LMark;
		last_alarm_time = this->RTC_Inst->now();
    LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
InterruptManager::InterruptManager(JsonArrayConst p)
	: InterruptManager(nullptr ) {}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::print_config() const
{
  LMark;
	Module::print_config();
  LMark;

	LPrintln("\tInterrupts Enabled  : ", (interrupts_enabled) ? "True" : "False" );
  LMark;

	// print out registered interrupts
	LPrintln("\tRegistered ISRs     : " );
  LMark;
	for (auto i = 0; i < 16; i++) {
   LMark;
		if(int_settings[i].pin != -1){
			LMark;
			LPrint("\t\tPin ", int_settings[i].pin, " | ISR: ", (int_settings[i].run_type == ISR_Type::IMMEDIATE) ? "Immediate" : "Check Flag");
			LMark;
			LPrintln(" | Type: ", interrupt_type_to_string(int_settings[i].type), " | ", (int_settings[i].enabled) ? "Enabled" : "Disabled" );
		}
	}

	// LPrint out registered timers
	LPrintln("\tRegistered Timers     : " );
  LMark;
	for (auto i = 0; i < MaxTimerCount; i++) {
    LMark;
		LPrint("\t\tTimer ", i, " : ");
    LMark;
		if (timer_settings[i].enabled) {
      LMark;
			unsigned long delay;
      LMark;
			AsyncDelay::units_t u;
      LMark;
			timers[i].getDelay(delay, u);
      LMark;
			LPrintln("[Enabled] Delay: ", delay);
      LMark;
		} else {
			LPrintln("[Disabled]");
      LMark;
		}
	}

	// LPrint out registered stopwatches
	LPrintln("\tRegistered StopWatches     : " );
  LMark;
	for (auto i = 0; i < MaxStopWatchCount; i++) {
    LMark;
		LPrint("\t\tStopwatch ", i, " : ");
    LMark;
		if (stopwatch_settings[i].enabled) {
      LMark;
			LPrintln("[Enabled] Elapsed: ",  millis()-stopwatch_settings[i].start_time);
      LMark;
		} else {
			LPrintln("[Disabled]");
      LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::print_state() const
{
  LMark;
	Module::print_state();
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::link_device_manager(Manager* LM)
{
  LMark;
	Module::link_device_manager(LM);
  LMark;

	if ( LM ){
    LMark;

		// Set manager's interrupt manager
		LM->interrupt_manager = this;
    LMark;

		// Get RTC from manager if needed
		if ( !RTC_Inst ) {
      LMark;
			RTC_Inst = LM->get_rtc_module();
      LMark;
		}

		// Link to sleep manager
		auto sleep_manager = LM->get_sleep_manager();
    LMark;
		if ( sleep_manager ) {
      LMark;
			link_sleep_manager(sleep_manager);
      LMark;
			sleep_manager->link_interrupt_manager(this);
      LMark;

		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::link_sleep_manager(SleepManager* SM)
{
  LMark;
	SleepMngr = SM;
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::run_pending_ISRs() {
  LMark;
	// Run any bottom half ISRs of interrupts
	run_ISR_bottom_halves();
  LMark;
	// Run 'ISR' functions for elapsed timers
	check_timers();
  LMark;
	// Run delayed ISR for elapsed interal timer
	run_pending_internal_timer_ISR();
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::set_enable_interrupt(const uint32_t pin, const bool state)
{
	LMark;
	int i = pin_to_interrupt(pin);
	LMark;
	if(i == 16) {
		LMark;
		print_module_label();
		LMark;
		LPrintln("Error: Pin ", pin, " is not valid");
		LMark;
		return;
  LMark;
	}
	LMark;
	if(int_settings[i].pin == pin) int_settings[i].enabled = state;
	else{
		LMark;
		print_module_label();
		LMark;
		LPrintln("Error: Pin ", pin, " has not been registered yet");
    LMark;

	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::register_ISR(const uint32_t pin, const ISRFuncPtr ISR, const uint32_t signal_type, const ISR_Type run_type)
{
	LMark;
	int i = pin_to_interrupt(pin);
	LMark;
	if(i == 16) {
		LMark;
		print_module_label();
		LMark;
		LPrintln("Error: Pin ", pin, " is not valid");
		LMark;
		return;
	}

	// Save interrupt details
	if(int_settings[i].pin == pin || int_settings[i].pin == -1){
   LMark;
		print_module_label();
    LMark;
		LPrint("Registering ISR on pin ", pin);
    LMark;
		LPrint(" to be triggered on ", signal_type);
    LMark;
		LPrintln(" and is ", (run_type==ISR_Type::IMMEDIATE) ? "immediate" : "delay" );
		LMark;
		int_settings[i] = { pin, ISR, signal_type, run_type, (ISR) ? true : false };
	}
 	else{
    LMark;
		print_module_label();
    LMark;
		LPrintln("Error: Pin ", pin, " cannot be attached since it conflicts with another interrupt pin");
    LMark;
		return;
	}

	// Set pin mode
	pinMode(pin, INPUT_PULLUP);
  LMark;

	// If ISR provided
	if (ISR != nullptr) {
    LMark;

		ISRFuncPtr tmpISR = (run_type==ISR_Type::IMMEDIATE) ? ISR : default_ISRs[i];
    LMark;

		attachInterrupt(digitalPinToInterrupt(pin), tmpISR, (signal_type<5) ? signal_type : 0 );
    LMark;
		attachInterrupt(digitalPinToInterrupt(pin), tmpISR, (signal_type<5) ? signal_type : 0 );
    LMark;

	}
	// If no ISR, detach interrupt pin
	else {
    LMark;
		detachInterrupt(digitalPinToInterrupt(pin));
    LMark;
	}

	// Ensure triggered flag false
	interrupt_triggered[i] = false;
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::reconnect_interrupt(const uint32_t pin)
{
  LMark;
	int i = pin_to_interrupt(pin);
  LMark;
	if(i == 16) {
    LMark;
		print_module_label();
    LMark;
		LPrintln("Error: Pin ", pin, " is not valid");
    LMark;
		return false;
	}
	IntDetails settings;
  LMark;
	if(int_settings[i].pin == pin) settings = int_settings[i];
	else{
    LMark;
		print_module_label();
    LMark;
		LPrintln("Error: Pin ", pin, " has not been registered yet");
    LMark;
		return false;
	}


	// Set pin mode
	// pinMode(pin, INPUT_PULLUP);  // was causing freezing

	// If ISR provided
	if (settings.ISR != nullptr) {
    LMark;
		ISRFuncPtr tmpISR = (settings.run_type == ISR_Type::IMMEDIATE) ? settings.ISR : default_ISRs[i];
    LMark;
		attachInterrupt(digitalPinToInterrupt(pin), tmpISR, (settings.type<5) ? settings.type : 0 );
    LMark;
		attachInterrupt(digitalPinToInterrupt(pin), tmpISR, (settings.type<5) ? settings.type : 0 );
    LMark;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::unregister_ISR(const uint32_t pin, const uint32_t signal_type)
{
  LMark;
	// Set interrupt to be the default
	register_ISR(pin, nullptr, signal_type, ISR_Type::IMMEDIATE);
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::run_ISR_bottom_halves()
{
  LMark;
	if (interrupts_enabled) {
    LMark;

		// For each interrupt pin
		for (int i = 0; i < 16; i++) {
      LMark;

			// If pin enabled and bottom half ISR provided
			// ISR will be Null if no interrupt on that pin,
			// or if ISR on pin is set to immediate (dont want to rerun the ISR here)
			if ( (interrupt_triggered[i]) && (int_settings[i].ISR != nullptr) ) {
        LMark;

				// LPrintln("I: ", i);

				// Run bottom half ISR
				int_settings[i].ISR();
        LMark;

			// currently not detaching in ISR... so nothing to reconnect
				// Reattach interrupts (disconnected in default ISR top half)
				// if (int_settings[pin].ISR != nullptr) {
				// 	// Attach interrupt with specified type
				// 	attachInterrupt(digitalPinToInterrupt(pin), int_settings[pin].ISR, (int_settings[pin].type<5) ? int_settings[pin].type : 0 );
				// }


				// // set triggered flag false
				interrupt_triggered[i] = false;
        LMark;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::interrupt_reset(const uint32_t pin)
{
  LMark;
	int i = pin_to_interrupt(pin);
  LMark;
	if(i == 16) {
    LMark;
		print_module_label();
    LMark;
		LPrintln("Error: Pin ", pin, " is not valid");
    LMark;
		return;
	}

  LMark;
	detachInterrupt(digitalPinToInterrupt(pin));
  LMark;
	delay(20);
  LMark;


	if (int_settings[i].pin == pin && int_settings[i].ISR != nullptr) {
    LMark;
		// Attach interrupt with specified type
		attachInterrupt(digitalPinToInterrupt(pin), int_settings[i].ISR, (int_settings[i].type<5) ? int_settings[i].type : 0 );
    LMark;
	}
	else {
    LMark;
		print_module_label();
    LMark;
		LPrintln("Error: Pin ", pin, " has not been registered yet");
    LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::RTC_alarm_duration(const TimeSpan duration)
{
  LMark;
	return (RTC_Inst) ? RTC_alarm_at(RTC_Inst->now() + duration) : false;
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::RTC_alarm_duration(const uint8_t days, const uint8_t hours, const uint8_t minutes, const uint8_t seconds)
{
  LMark;
	return RTC_alarm_duration( TimeSpan(days, hours, minutes, seconds) );
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::RTC_alarm_at(DateTime future_time)
{
  LMark;
	// Don't sleep if no RTC to wake up device
	if (RTC_Inst == nullptr) {
    LMark;
		return false;
	}

	print_module_label();
  LMark;
	LPrint("Will set alarm for : ");
  LMark;
	RTC_Inst->print_DateTime(future_time);
  LMark;

	// Don't set alarm at time in the past
	DateTime now = RTC_Inst->now();
  LMark;
	if ( (future_time - now).totalseconds() < 0 ) {
    LMark;
		print_module_label();
    LMark;
		LPrintln("Wont wake from alarm in the past, increasing time to following day");
    LMark;
		// future_time = future + TimeSpan(1,0,0,0);    // might not work if DateTime is several days in past

		// Adjust future_time to be following day at same time intended
		future_time = DateTime(	now.year(),
								now.month(),
								now.day(),
								future_time.hour(),
								future_time.minute(),
								future_time.second() )
								+ TimeSpan(1,0,0,0);
    LMark;

		print_module_label();
    LMark;
		LPrint("Will instead try to set alarm for : ");
    LMark;
		RTC_Inst->print_DateTime(future_time);
    LMark;

	}

	// Tell RTC_Inst to set RTC time at future_time
	// Then call sleep_until_interrupt on pin, because that is what it is
	RTC_Inst->set_alarm(future_time);
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::RTC_alarm_at(const uint8_t hour, const uint8_t minute, const uint8_t second)
{
  LMark;
	// Don't sleep if no RTC to wake up device
	if (RTC_Inst == nullptr) {
    LMark;
		return false;
	}

	// Call RTC_alarm_at(DateTime future_time) with that time today
	// That function will adjust to following day if necessary
	DateTime now = RTC_Inst->now();
  LMark;
	return RTC_alarm_at( DateTime(now.year(), now.month(), now.day(), hour, minute, second) );
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::RTC_alarm_duration_from_last(const TimeSpan duration)
{
  LMark;
	return (RTC_Inst) ? RTC_alarm_at(last_alarm_time + duration) : false;
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::RTC_alarm_duration_from_last(const uint8_t days, const uint8_t hours, const uint8_t minutes, const uint8_t seconds)
{
  LMark;
	return RTC_alarm_duration_from_last( TimeSpan(days, hours, minutes, seconds) );
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::check_timers()
{
  LMark;
	// Check each timer
	for (auto i = 0; i < MaxTimerCount; i++) {
    LMark;
		// If enabled and expired
		if ( (timer_settings[i].enabled) && (timers[i].isExpired()) ){
      LMark;
			// Run associated ISR
			timer_settings[i].ISR();
      LMark;

			// If set to repeat, start again, else disable
			if (timer_settings[i].repeat) {
        LMark;
				timers[i].repeat();
        LMark;
			} else {
				clear_timer(i);
        LMark;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::register_timer(const uint8_t timer_num, const unsigned long duration, const ISRFuncPtr ISR, const bool repeat)
{
  LMark;
	if (timer_num < MaxTimerCount) {
    LMark;
		timer_settings[timer_num] = { ISR, duration, repeat, true };
		timers[timer_num].start(duration, AsyncDelay::MILLIS);
    LMark;
	} else {
		print_module_label();
    LMark;
		LPrintln("Timer number out of range");
    LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::clear_timer(const uint8_t timer_num)
{
  LMark;
	if (timer_num < MaxTimerCount) {
    LMark;
		if (print_verbosity == Verbosity::V_HIGH) {
      LMark;
			print_module_label();
      LMark;
			LPrintln("Clear timer ", timer_num);
      LMark;
		}
		timers[timer_num].expire();
    LMark;
		timer_settings[timer_num].enabled = false;
    LMark;
	} else {
		print_module_label();
    LMark;
		LPrintln("Timer number out of range");
    LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::register_internal_timer(const uint duration, const ISRFuncPtr ISR, const bool repeat, const ISR_Type run_type)
{
  LMark;
	internal_timer.ISR		= ISR;
  LMark;
	internal_timer.run_type	= run_type;
  LMark;
	internal_timer.duration	= duration;
  LMark;
	internal_timer.repeat	= repeat;
  LMark;
	internal_timer.enabled	= true;
  LMark;

	if (run_type == ISR_Type::IMMEDIATE) {
    LMark;
		rtcCounter.attachInterrupt(ISR);
    LMark;
	} else {
    LMark;
		rtcCounter.detachInterrupt();
    LMark;
	}

	if (repeat) {
    LMark;
		rtcCounter.setPeriodicAlarm(duration);
    LMark;
	} else {
		// Timer is for exact time, need to offset by adding current time
		rtcCounter.setAlarmEpoch(duration + rtcCounter.getEpoch());
    LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::run_pending_internal_timer_ISR()
{
  LMark;
	if ( (internal_timer.run_type == ISR_Type::CHECK_FLAG) &&
		 (internal_timer.ISR != nullptr) &&
		 (rtcCounter.getFlag()) ) {
    LMark;
		rtcCounter.clearFlag();
    LMark;
		LPrintln("Executing ISR");
    LMark;
		internal_timer.ISR();
    LMark;
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::internal_timer_enable(const bool enable)
{
  LMark;
	internal_timer.enabled = enable;
  LMark;

	if (enable) {
		// Simply re-register with previous settings
		register_internal_timer(
			internal_timer.duration,
			internal_timer.ISR,
			internal_timer.repeat,
			internal_timer.run_type
		);
    LMark;
	} else {
		rtcCounter.disableAlarm();
    LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::unregister_internal_timer()
{
  LMark;
	rtcCounter.disableAlarm();
  LMark;
	rtcCounter.detachInterrupt();
  LMark;

	internal_timer.ISR		= nullptr;
  LMark;
	internal_timer.run_type	= ISR_Type::IMMEDIATE;
  LMark;
	internal_timer.duration	= 0;
  LMark;
	internal_timer.repeat	= false;
  LMark;
	internal_timer.enabled	= false;
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
uint32_t InterruptManager::pin_to_interrupt(uint32_t pin){
  LMark;
	if(pin == 11) return 0;
	else if(pin == 13) return 1;
	else if(pin == 10 || pin == A0 || pin == A5) return 2;
	else if(pin == 12) return 3;
	else if(pin == 6 || pin == A3) return 4;
	else if(pin == A4) return 5;
	else if(pin == SDA) return 6;
	else if(pin == 9 || pin == SCL) return 7;
	else if(pin == A1) return 8;
	else if(pin == A2) return 9;
	else if(pin == 23 || pin == 1) return 10;
	else if(pin == 24 || pin == 0) return 11;
	else if(pin == 5) return 15;
	else return 16;
}


///////////////////////////////////////////////////////////////////////////////
