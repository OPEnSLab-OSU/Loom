///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Interrupt_Manager.cpp
/// @brief		File for Loom_Interrupt_Manager implementation
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Interrupt_Manager.h"
#include "Sleep_Manager.h"
#include "RTC/RTC.h"
#include "Manager.h"

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>


///////////////////////////////////////////////////////////////////////////////

bool Loom_Interrupt_Manager::interrupt_triggered[InteruptRange] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

///////////////////////////////////////////////////////////////////////////////

const ISRFuncPtr Loom_Interrupt_Manager::default_ISRs[InteruptRange] =
{
	default_ISR_0,  default_ISR_1,  default_ISR_2,  default_ISR_3,
	default_ISR_4,  default_ISR_5,  default_ISR_6,  default_ISR_7,
	default_ISR_8,  default_ISR_9,  default_ISR_10, default_ISR_11,
	default_ISR_12, default_ISR_13, default_ISR_14, default_ISR_15
};

///////////////////////////////////////////////////////////////////////////////
const char* Loom_Interrupt_Manager::interrupt_type_to_string(const uint8_t type)
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
Loom_Interrupt_Manager::Loom_Interrupt_Manager(
		LoomManager* manager,
		LoomRTC*		RTC_Inst
	)
	: LoomModule(manager, "InterruptManager", Type::Interrupt_Manager )
	, RTC_Inst(RTC_Inst)
{

	interrupts_enabled = true;

	for (auto i = 0; i < InteruptRange; i++) {
		int_settings[i] = {-1, nullptr, 0, ISR_Type::IMMEDIATE, true};
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
Loom_Interrupt_Manager::Loom_Interrupt_Manager(LoomManager* manager, JsonArrayConst p)
	: Loom_Interrupt_Manager(manager, nullptr ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::print_config() const
{
	LoomModule::print_config();

	LPrintln("\tInterrupts Enabled  : ", (interrupts_enabled) ? "True" : "False" );

	// print out registered interrupts
	LPrintln("\tRegistered ISRs     : " );
	for (auto i = 0; i < 16; i++) {
		if(int_settings[i].pin != -1){
			LPrint("\t\tPin ", int_settings[i].pin, " | ISR: ", (int_settings[i].run_type == ISR_Type::IMMEDIATE) ? "Immediate" : "Check Flag");
			LPrintln(" | Type: ", interrupt_type_to_string(int_settings[i].type), " | ", (int_settings[i].enabled) ? "Enabled" : "Disabled" );
		}
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
void Loom_Interrupt_Manager::print_state() const
{
	LoomModule::print_state();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::link_device_manager(LoomManager* LM)
{
	LoomModule::link_device_manager(LM);

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
void Loom_Interrupt_Manager::link_sleep_manager(Loom_Sleep_Manager* SM)
{
	Sleep_Manager = SM;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::run_pending_ISRs() {
	// Run any bottom half ISRs of interrupts
	run_ISR_bottom_halves();
	// Run 'ISR' functions for elapsed timers
	check_timers();
	// Run delayed ISR for elapsed interal timer
	run_pending_internal_timer_ISR();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::set_enable_interrupt(const uint32_t pin, const bool state)
{
	int i = pin_to_interrupt(pin);
	if(i == 16) {
		print_module_label();
		LPrintln("Error: Pin ", pin, " is not valid");
		return;
	}
	if(int_settings[i].pin == pin) int_settings[i].enabled = state;
	else{
		print_module_label();
		LPrintln("Error: Pin ", pin, " has not been registered yet");
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::register_ISR(const uint32_t pin, const ISRFuncPtr ISR, const uint32_t signal_type, const ISR_Type run_type)
{
	int i = pin_to_interrupt(pin);
	if(i == 16) {
		print_module_label();
		LPrintln("Error: Pin ", pin, " is not valid");
		return;
	}

	// Save interrupt details
	if(int_settings[i].pin == pin || int_settings[i].pin == -1){
		print_module_label();
		LPrint("Registering ISR on pin ", pin);
		LPrint(" to be triggered on ", signal_type);
		LPrintln(" and is ", (run_type==ISR_Type::IMMEDIATE) ? "immediate" : "delay" );
		int_settings[i] = { pin, ISR, signal_type, run_type, (ISR) ? true : false };
	}
 	else{
		print_module_label();
		LPrintln("Error: Pin ", pin, " cannot be attached since it conflicts with another interrupt pin");
		return;
	}

	// Set pin mode
	pinMode(pin, INPUT_PULLUP);

	// If ISR provided
	if (ISR != nullptr) {

		ISRFuncPtr tmpISR = (run_type==ISR_Type::IMMEDIATE) ? ISR : default_ISRs[i];

		attachInterrupt(digitalPinToInterrupt(pin), tmpISR, (signal_type<5) ? signal_type : 0 );
		attachInterrupt(digitalPinToInterrupt(pin), tmpISR, (signal_type<5) ? signal_type : 0 );

	}
	// If no ISR, detach interrupt pin
	else {
		detachInterrupt(digitalPinToInterrupt(pin));
	}

	// Ensure triggered flag false
	interrupt_triggered[i] = false;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Interrupt_Manager::reconnect_interrupt(const uint32_t pin)
{
	int i = pin_to_interrupt(pin);
	if(i == 16) {
		print_module_label();
		LPrintln("Error: Pin ", pin, " is not valid");
		return false;
	}
	IntDetails settings;
	if(int_settings[i].pin == pin) settings = int_settings[i];
	else{
		print_module_label();
		LPrintln("Error: Pin ", pin, " has not been registered yet");
		return false;
	}


	// Set pin mode
	// pinMode(pin, INPUT_PULLUP);  // was causing freezing

	// If ISR provided
	if (settings.ISR != nullptr) {
		ISRFuncPtr tmpISR = (settings.run_type == ISR_Type::IMMEDIATE) ? settings.ISR : default_ISRs[i];
		attachInterrupt(digitalPinToInterrupt(pin), tmpISR, (settings.type<5) ? settings.type : 0 );
		attachInterrupt(digitalPinToInterrupt(pin), tmpISR, (settings.type<5) ? settings.type : 0 );
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::unregister_ISR(const uint32_t pin, const uint32_t signal_type)
{
	// Set interrupt to be the default
	register_ISR(pin, nullptr, signal_type, ISR_Type::IMMEDIATE);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::run_ISR_bottom_halves()
{
	if (interrupts_enabled) {

		// For each interrupt pin
		for (int i = 0; i < 16; i++) {

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
void Loom_Interrupt_Manager::interrupt_reset(const uint32_t pin)
{
	int i = pin_to_interrupt(pin);
	if(i == 16) {
		print_module_label();
		LPrintln("Error: Pin ", pin, " is not valid");
		return;
	}

	detachInterrupt(digitalPinToInterrupt(pin));
	delay(20);


	if (int_settings[i].pin == pin && int_settings[i].ISR != nullptr) {
		// Attach interrupt with specified type
		attachInterrupt(digitalPinToInterrupt(pin), int_settings[i].ISR, (int_settings[i].type<5) ? int_settings[i].type : 0 );
	}
	else {
		print_module_label();
		LPrintln("Error: Pin ", pin, " has not been registered yet");
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Interrupt_Manager::RTC_alarm_duration(const TimeSpan duration)
{
	return (RTC_Inst) ? RTC_alarm_at(RTC_Inst->now() + duration) : false;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Interrupt_Manager::RTC_alarm_duration(const uint8_t days, const uint8_t hours, const uint8_t minutes, const uint8_t seconds)
{
	return RTC_alarm_duration( TimeSpan(days, hours, minutes, seconds) );
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Interrupt_Manager::RTC_alarm_at(DateTime future_time)
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
bool Loom_Interrupt_Manager::RTC_alarm_at(const uint8_t hour, const uint8_t minute, const uint8_t second)
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
bool Loom_Interrupt_Manager::RTC_alarm_duration_from_last(const TimeSpan duration)
{
	return (RTC_Inst) ? RTC_alarm_at(last_alarm_time + duration) : false;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Interrupt_Manager::RTC_alarm_duration_from_last(const uint8_t days, const uint8_t hours, const uint8_t minutes, const uint8_t seconds)
{
	return RTC_alarm_duration_from_last( TimeSpan(days, hours, minutes, seconds) );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::check_timers()
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
void Loom_Interrupt_Manager::register_timer(const uint8_t timer_num, const unsigned long duration, const ISRFuncPtr ISR, const bool repeat)
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
void Loom_Interrupt_Manager::clear_timer(const uint8_t timer_num)
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
void Loom_Interrupt_Manager::register_internal_timer(const uint duration, const ISRFuncPtr ISR, const bool repeat, const ISR_Type run_type)
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
bool Loom_Interrupt_Manager::run_pending_internal_timer_ISR()
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
void Loom_Interrupt_Manager::internal_timer_enable(const bool enable)
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
void Loom_Interrupt_Manager::unregister_internal_timer()
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
uint32_t Loom_Interrupt_Manager::pin_to_interrupt(uint32_t pin){
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
