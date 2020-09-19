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

REGISTER(Module, InterruptManager, "InterruptManager");

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
		RTC* RTC_Inst
	) 
	: Module("InterruptManager", Type::InterruptManager )
	, RTC_Inst(RTC_Inst)
{

	interrupts_enabled = true;

	for (auto i = 0; i < InteruptRange; i++) {
		int_settings[i] = {nullptr, 0, ISR_Type::IMMEDIATE, true};
	}
	for (auto i = 0; i < MaxTimerCount; i++) {
		timer_settings[i] = {nullptr, 0, false, false};
	}
	for (auto i = 0; i < MaxStopWatchCount; i++) {
		stopwatch_settings[i] = {0, false};
	}

	// Setup the RTCCounter for internal timer
	rtcCounter.begin();

	// Note initial wake time
	if (this->RTC_Inst != nullptr) {
		last_alarm_time = this->RTC_Inst->now();
	}
}

///////////////////////////////////////////////////////////////////////////////
InterruptManager::InterruptManager(JsonArrayConst p)
	: InterruptManager(nullptr ) {}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::print_config() const
{
	Module::print_config();

	LPrintln("\tInterrupts Enabled  : ", (interrupts_enabled) ? "True" : "False" );

	// print out registered interrupts
	LPrintln("\tRegistered ISRs     : " );
	for (auto i = 0; i < InteruptRange; i++) {
		LPrint("\t\tPin ", i, " | ISR: ", (int_settings[i].run_type == ISR_Type::IMMEDIATE) ? "Immediate" : "Check Flag");
		LPrintln(" | Type: ", interrupt_type_to_string(int_settings[i].type), " | ", (int_settings[i].enabled) ? "Enabled" : "Disabled" );

	}

	// LPrint out registered timers
	LPrintln("\tRegistered Timers     : " );
	for (auto i = 0; i < MaxTimerCount; i++) {
		LPrint("\t\tTimer ", i, " : ");
		if (timer_settings[i].enabled) {
			unsigned long delay;
			AsyncDelay::units_t u;
			timers[i].getDelay(delay, u);
			LPrintln("[Enabled] Delay: ", delay);
		} else {
			LPrintln("[Disabled]");
		}
	}

	// LPrint out registered stopwatches
	LPrintln("\tRegistered StopWatches     : " );
	for (auto i = 0; i < MaxStopWatchCount; i++) {
		LPrint("\t\tStopwatch ", i, " : ");
		if (stopwatch_settings[i].enabled) {
			LPrintln("[Enabled] Elapsed: ",  millis()-stopwatch_settings[i].start_time);
		} else {
			LPrintln("[Disabled]");
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::print_state() const
{
	Module::print_state();
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::link_device_manager(Manager* LM)
{
	Module::link_device_manager(LM);

	if ( LM ){

		// Set manager's interrupt manager 
		LM->interrupt_manager = this;

		// Get RTC from manager if needed
		if ( !RTC_Inst ) {
			RTC_Inst = LM->get_rtc_module();
		}

		// Link to sleep manager
		auto sleep_manager = LM->get_sleep_manager(); 
		if ( sleep_manager ) {
			link_sleep_manager(sleep_manager);
			sleep_manager->link_interrupt_manager(this);

		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::link_sleep_manager(SleepManager* SM)
{
	SleepMngr = SM;
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::run_pending_ISRs() {
	// Run any bottom half ISRs of interrupts
	run_ISR_bottom_halves();
	// Run 'ISR' functions for elapsed timers
	check_timers();
	// Run delayed ISR for elapsed interal timer
	run_pending_internal_timer_ISR();
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::set_enable_interrupt(const uint32_t pin, const bool state)
{
	if (pin < InteruptRange) {
		int_settings[pin].enabled = state;
	} 
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::register_ISR(const uint32_t pin, const ISRFuncPtr ISR, const uint32_t signal_type, const ISR_Type run_type)
{
	if (pin < InteruptRange) {

		print_module_label();
		LPrint("Registering ISR on pin ", pin);
		LPrint(" to be triggered on ", signal_type);
		LPrintln(" and is ", (run_type==ISR_Type::IMMEDIATE) ? "immediate" : "delay" );

		// Save interrupt details
		int_settings[pin] = { ISR, signal_type, run_type, (ISR) ? true : false };

		// Set pin mode
		pinMode(pin, INPUT_PULLUP);

		// If ISR provided
		if (ISR != nullptr) {

			ISRFuncPtr tmpISR = (run_type==ISR_Type::IMMEDIATE) ? ISR : default_ISRs[pin];

			attachInterrupt(digitalPinToInterrupt(pin), tmpISR, (signal_type<5) ? signal_type : 0 );
			attachInterrupt(digitalPinToInterrupt(pin), tmpISR, (signal_type<5) ? signal_type : 0 );

		} 
		// If no ISR, detach interrupt pin
		else {
			detachInterrupt(digitalPinToInterrupt(pin));
		}

		// Ensure triggered flag false 
		interrupt_triggered[pin] = false;
	}
	else {
		LPrintln("Cannot register an interrupt on pin number greater than 16");
		// TODO: THROW ERROR
	}
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::reconnect_interrupt(const uint32_t pin)
{
	IntDetails settings = int_settings[pin];

	// Set pin mode
	// pinMode(pin, INPUT_PULLUP);  // was causing freezing

	// If ISR provided
	if (settings.ISR != nullptr) {
		ISRFuncPtr tmpISR = (settings.run_type == ISR_Type::IMMEDIATE) ? settings.ISR : default_ISRs[pin];
		attachInterrupt(digitalPinToInterrupt(pin), tmpISR, (settings.type<5) ? settings.type : 0 );
		attachInterrupt(digitalPinToInterrupt(pin), tmpISR, (settings.type<5) ? settings.type : 0 );
	} 
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::unregister_ISR(const uint32_t pin, const uint32_t signal_type)
{
	// Set interrupt to be the default
	if (pin < InteruptRange) {
		register_ISR(pin, nullptr, signal_type, ISR_Type::IMMEDIATE);
	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::run_ISR_bottom_halves()
{
	if (interrupts_enabled) {

		// For each interrupt pin
		for (int i = 0; i < InteruptRange; i++) {

			// If pin enabled and bottom half ISR provided
			// ISR will be Null if no interrupt on that pin,
			// or if ISR on pin is set to immediate (dont want to rerun the ISR here)
			if ( (interrupt_triggered[i]) && (int_settings[i].ISR != nullptr) ) {

				// LPrintln("I: ", i);

				// Run bottom half ISR
				int_settings[i].ISR();

			// currently not detaching in ISR... so nothing to reconnect
				// Reattach interrupts (disconnected in default ISR top half)
				// if (int_settings[pin].ISR != nullptr) {
				// 	// Attach interrupt with specified type
				// 	attachInterrupt(digitalPinToInterrupt(pin), int_settings[pin].ISR, (int_settings[pin].type<5) ? int_settings[pin].type : 0 );
				// }


				// // set triggered flag false 
				interrupt_triggered[i] = false;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::interrupt_reset(const uint32_t pin)
{
	detachInterrupt(digitalPinToInterrupt(pin));
	delay(20);

	if (int_settings[pin].ISR != nullptr) {
		// Attach interrupt with specified type
		attachInterrupt(digitalPinToInterrupt(pin), int_settings[pin].ISR, (int_settings[pin].type<5) ? int_settings[pin].type : 0 );
	}
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::RTC_alarm_duration(const TimeSpan duration)
{
	return (RTC_Inst) ? RTC_alarm_at(RTC_Inst->now() + duration) : false;
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::RTC_alarm_duration(const uint8_t days, const uint8_t hours, const uint8_t minutes, const uint8_t seconds)
{
	return RTC_alarm_duration( TimeSpan(days, hours, minutes, seconds) );
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::RTC_alarm_at(DateTime future_time)
{
	// Don't sleep if no RTC to wake up device
	if (RTC_Inst == nullptr) {
		return false;
	}

	print_module_label();
	LPrint("Will set alarm for : ");
	RTC_Inst->print_DateTime(future_time);

	// Don't set alarm at time in the past
	DateTime now = RTC_Inst->now();
	if ( (future_time - now).totalseconds() < 0 ) {
		print_module_label();
		LPrintln("Wont wake from alarm in the past, increasing time to following day");
		// future_time = future + TimeSpan(1,0,0,0);    // might not work if DateTime is several days in past
		
		// Adjust future_time to be following day at same time intended
		future_time = DateTime(	now.year(), 
								now.month(), 
								now.day(), 
								future_time.hour(), 
								future_time.minute(), 
								future_time.second() )
								+ TimeSpan(1,0,0,0);

		print_module_label();
		LPrint("Will instead try to set alarm for : ");
		RTC_Inst->print_DateTime(future_time);

	}

	// Tell RTC_Inst to set RTC time at future_time
	// Then call sleep_until_interrupt on pin, because that is what it is
	RTC_Inst->set_alarm(future_time);
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::RTC_alarm_at(const uint8_t hour, const uint8_t minute, const uint8_t second)
{
	// Don't sleep if no RTC to wake up device
	if (RTC_Inst == nullptr) {
		return false;
	}

	// Call RTC_alarm_at(DateTime future_time) with that time today
	// That function will adjust to following day if necessary
	DateTime now = RTC_Inst->now();
	return RTC_alarm_at( DateTime(now.year(), now.month(), now.day(), hour, minute, second) ); 
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::RTC_alarm_duration_from_last(const TimeSpan duration)
{
	return (RTC_Inst) ? RTC_alarm_at(last_alarm_time + duration) : false;
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::RTC_alarm_duration_from_last(const uint8_t days, const uint8_t hours, const uint8_t minutes, const uint8_t seconds)
{
	return RTC_alarm_duration_from_last( TimeSpan(days, hours, minutes, seconds) );
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::check_timers()
{
	// Check each timer
	for (auto i = 0; i < MaxTimerCount; i++) {
		// If enabled and expired
		if ( (timer_settings[i].enabled) && (timers[i].isExpired()) ){
			// Run associated ISR
			timer_settings[i].ISR();
			
			// If set to repeat, start again, else disable
			if (timer_settings[i].repeat) {
				timers[i].repeat();
			} else {
				clear_timer(i);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::register_timer(const uint8_t timer_num, const unsigned long duration, const ISRFuncPtr ISR, const bool repeat)
{
	if (timer_num < MaxTimerCount) {
		timer_settings[timer_num] = { ISR, duration, repeat, true };
		timers[timer_num].start(duration, AsyncDelay::MILLIS);
	} else {
		print_module_label();
		LPrintln("Timer number out of range");
	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::clear_timer(const uint8_t timer_num)
{
	if (timer_num < MaxTimerCount) {
		if (print_verbosity == Verbosity::V_HIGH) {
			print_module_label();
			LPrintln("Clear timer ", timer_num);
		}
		timers[timer_num].expire();
		timer_settings[timer_num].enabled = false;
	} else {
		print_module_label();
		LPrintln("Timer number out of range");
	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::register_internal_timer(const uint duration, const ISRFuncPtr ISR, const bool repeat, const ISR_Type run_type)		
{
	internal_timer.ISR		= ISR;
	internal_timer.run_type	= run_type;
	internal_timer.duration	= duration;
	internal_timer.repeat	= repeat;
	internal_timer.enabled	= true;

	if (run_type == ISR_Type::IMMEDIATE) {
		rtcCounter.attachInterrupt(ISR);
	} else {
		rtcCounter.detachInterrupt();
	}

	if (repeat) {
		rtcCounter.setPeriodicAlarm(duration);
	} else {
		// Timer is for exact time, need to offset by adding current time
		rtcCounter.setAlarmEpoch(duration + rtcCounter.getEpoch());
	}
}

///////////////////////////////////////////////////////////////////////////////
bool InterruptManager::run_pending_internal_timer_ISR()
{
	if ( (internal_timer.run_type == ISR_Type::CHECK_FLAG) &&
		 (internal_timer.ISR != nullptr) &&
		 (rtcCounter.getFlag()) ) {
		rtcCounter.clearFlag();
		LPrintln("Executing ISR");
		internal_timer.ISR();
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::internal_timer_enable(const bool enable)
{
	internal_timer.enabled = enable;

	if (enable) {
		// Simply re-register with previous settings
		register_internal_timer(
			internal_timer.duration,
			internal_timer.ISR,
			internal_timer.repeat,
			internal_timer.run_type
		);
	} else {
		rtcCounter.disableAlarm();
	}
}

///////////////////////////////////////////////////////////////////////////////
void InterruptManager::unregister_internal_timer()
{
	rtcCounter.disableAlarm();  
	rtcCounter.detachInterrupt();

	internal_timer.ISR		= nullptr;
	internal_timer.run_type	= ISR_Type::IMMEDIATE;
	internal_timer.duration	= 0;
	internal_timer.repeat	= false;
	internal_timer.enabled	= false;
}

///////////////////////////////////////////////////////////////////////////////
