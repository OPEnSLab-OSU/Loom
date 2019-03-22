
#include "Loom_Interrupt_Manager.h"
#include "RTC/Loom_RTC.h"

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>


bool Loom_Interrupt_Manager::interrupt_triggered[InteruptRange] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


const ISRFuncPtr Loom_Interrupt_Manager::default_ISRs[InteruptRange] =
{
	default_ISR_0,  default_ISR_1,  default_ISR_2,  default_ISR_3,
	default_ISR_4,  default_ISR_5,  default_ISR_6,  default_ISR_7,
	default_ISR_8,  default_ISR_9,  default_ISR_10, default_ISR_11,
	default_ISR_12, default_ISR_13, default_ISR_14, default_ISR_15
};


/////////////////////////////////////////////////////////////////////
Loom_Interrupt_Manager::Loom_Interrupt_Manager( 
		char*		module_name, 
		LoomRTC*	RTC_Inst
	) 
	: LoomModule( module_name )
{
	interrupts_enabled = true;

	for (auto i = 0; i < InteruptRange; i++) {
		int_settings[i] = {NULL, 0, true, false};
	}
	for (auto i = 0; i < MaxTimerCount; i++) {
		timer_settings[i] = {NULL, 0, false, false};
	}
	for (auto i = 0; i < MaxStopWatchCount; i++) {
		stopwatch_settings[i] = {0, false};
	}

	this->RTC_Inst = RTC_Inst;

	if (this->RTC_Inst != NULL) {
		last_alarm_time = this->RTC_Inst->now();
	}
}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_Interrupt_Manager::~Loom_Interrupt_Manager()
{

}

/////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::print_config()
{
	LoomModule::print_config();

	Println3('\t', "Interrupts Enabled  : ", (interrupts_enabled) ? "True" : "False" );

	// print out registered interrupts
	Println2('\t', "Registered ISRs     : " );
	for (auto i = 0; i < InteruptRange; i++) {
		Print5("\t\t", "Pin ", i, " | ISR: ", (int_settings[i].is_immediate) ? "Immediate" : "Bottom Half");
		Println4(" | Type: ", int_settings[i].type, " | ", (int_settings[i].enabled) ? "Enabled" : "Disabled" );

	}

	Println2('\t', "Registered Timers     : " );
	for (auto i = 0; i < MaxTimerCount; i++) {
		Print4("\t\t", "Timer ", i, " : ");
		if (timer_settings[i].enabled) {
			unsigned long delay;
			AsyncDelay::units_t u;
			timers[i].getDelay(delay, u);
			Println2("[Enabled] Delay: ", delay);
		} else {
			Println("[Disabled]");
		}
	}

	Println2('\t', "Registered StopWatches     : " );
	for (auto i = 0; i < MaxStopWatchCount; i++) {
		Print4("\t\t", "Stopwatch ", i, " : ");
		if (stopwatch_settings[i].enabled) {
			Println2("[Enabled] Elapsed: ",  millis()-stopwatch_settings[i].start_time);
		} else {
			Println("[Disabled]");
		}
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::print_state()
{
	LoomModule::print_state();
}


/////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::link_device_manager(LoomManager* LM)
{
	LoomModule::link_device_manager(LM);

	// If no currently linked RTC object, try to get one from Manager
	if ( (RTC_Inst == NULL) && (LM != NULL) ){
		RTC_Inst = LM->get_rtc_module();
	}
}


/////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::run_pending_ISRs() {
	// Run any bottom half ISRs of interrupts
	run_ISR_bottom_halves();
	// Run 'ISR' functions for elapsed timers
	check_timers();
}

/////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::set_interrupts_enabled(bool state)
{
	interrupts_enabled = state;
}

/////////////////////////////////////////////////////////////////////
bool Loom_Interrupt_Manager::get_interrupts_enabled()
{
	return interrupts_enabled;
}

/////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::set_enable_interrupt(byte pin, bool state)
{
	if (pin < InteruptRange) {
		int_settings[pin].enabled = state;
	} 
}

/////////////////////////////////////////////////////////////////////
bool Loom_Interrupt_Manager::get_enable_interrupt(byte pin)
{
	return (pin < InteruptRange) ? int_settings[pin].enabled : false; 
}

/////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::register_ISR(byte pin, ISRFuncPtr ISR, byte type, bool immediate)
{
	if (pin < InteruptRange) {

		print_module_label();
		Print2("Registering ISR on pin ", pin);
		Print2(" to be triggered on ", type);
		Println2(" and is ", (immediate) ? "immediate" : "delay" );


		// Save interrupt details
		int_settings[pin] = { ISR, type, immediate, (ISR) ? true : false };

		// Set pin mode
		pinMode(pin, INPUT_PULLUP);


		// If ISR provided
		if (ISR != NULL) {

			ISRFuncPtr tmpISR = (immediate) ? ISR : default_ISRs[pin];

			// Attach default interrupt with specified type
			// switch( type ) {
			// 	case INT_LOW     : attachInterrupt(digitalPinToInterrupt(pin), tmpISR, LOW); break;
			// 	case INT_HIGH    : attachInterrupt(digitalPinToInterrupt(pin), tmpISR, HIGH); break; 
			// 	case INT_CHANGE  : attachInterrupt(digitalPinToInterrupt(pin), tmpISR, CHANGE); break;
			// 	case INT_FALLING : attachInterrupt(digitalPinToInterrupt(pin), tmpISR, FALLING); break;
			// 	case INT_RISING  : attachInterrupt(digitalPinToInterrupt(pin), tmpISR, RISING); break;
			// }
			attachInterrupt(digitalPinToInterrupt(pin), tmpISR, (type<5) ? type : 0 );

		} 
		// If no ISR, detach interrupt pin
		else {
			detachInterrupt(digitalPinToInterrupt(pin));
		}

		// Ensure triggered flag false 
		interrupt_triggered[pin] = false;
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::unregister_ISR(byte pin, byte type)
{
	// Set interrupt to be the default
	if (pin < InteruptRange) {
		register_ISR(pin, NULL, type, true);
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::run_ISR_bottom_halves()
{
	if (interrupts_enabled) {

		// For each interrupt pin
		for (int i = 0; i < InteruptRange; i++) {

			// If pin enabled and bottom half ISR provided
			// ISR will be Null if no interrupt on that pin,
			// or if ISR on pin is set to immediate (dont want to rerun the ISR here)
			if ( (interrupt_triggered[i]) && (int_settings[i].ISR != NULL) ) {

				Println2("I: ", i);


				// Run bottom half ISR
				int_settings[i].ISR();

				// Reattach interrupts (disconnected in default ISR top half)
				// switch( int_settings[i].type ) {
				// 	case INT_LOW     : attachInterrupt(digitalPinToInterrupt(i), default_ISRs[i], LOW); break;
				// 	case INT_HIGH    : attachInterrupt(digitalPinToInterrupt(i), default_ISRs[i], HIGH); break; 
				// 	case INT_CHANGE  : attachInterrupt(digitalPinToInterrupt(i), default_ISRs[i], CHANGE); break;
				// 	case INT_FALLING : attachInterrupt(digitalPinToInterrupt(i), default_ISRs[i], FALLING); break;
				// 	case INT_RISING  : attachInterrupt(digitalPinToInterrupt(i), default_ISRs[i], RISING); break;
				// }

				// // set triggered flag false 
				interrupt_triggered[i] = false;
			}
							// set triggered flag false 
				// interrupt_triggered[i] = false;

		}
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::interrupt_reset(byte pin)
{
	detachInterrupt(digitalPinToInterrupt(pin));
	delay(20);

	if (int_settings[pin].ISR != NULL) {

		// Attach interrupt with specified type
		// switch( int_settings[pin].type ) {
		// 	case INT_LOW     : attachInterrupt(digitalPinToInterrupt(pin), int_settings[pin].ISR, LOW);
		// 	case INT_HIGH    : attachInterrupt(digitalPinToInterrupt(pin), int_settings[pin].ISR, HIGH); 
		// 	case INT_RISING  : attachInterrupt(digitalPinToInterrupt(pin), int_settings[pin].ISR, RISING);
		// 	case INT_FALLING : attachInterrupt(digitalPinToInterrupt(pin), int_settings[pin].ISR, FALLING);
		// 	case INT_CHANGE  : attachInterrupt(digitalPinToInterrupt(pin), int_settings[pin].ISR, CHANGE);
		// }
		attachInterrupt(digitalPinToInterrupt(pin), int_settings[pin].ISR, (int_settings[pin].type<5) ? int_settings[pin].type : 0 );

	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::set_RTC_module(LoomRTC* RTC_Inst)
{
	this->RTC_Inst = RTC_Inst;
}

/////////////////////////////////////////////////////////////////////
LoomRTC* Loom_Interrupt_Manager::get_RTC_module()
{
	return RTC_Inst;
}

/////////////////////////////////////////////////////////////////////
bool Loom_Interrupt_Manager::RTC_alarm_relative(TimeSpan duration)
{
	return (RTC_Inst) ? RTC_alarm_exact(RTC_Inst->now() + duration) : false;
}

/////////////////////////////////////////////////////////////////////
bool Loom_Interrupt_Manager::RTC_alarm_relative(uint days, uint hours, uint minutes, uint seconds)
{
	return RTC_alarm_relative( TimeSpan(days, hours, minutes, seconds) );
}



// bool Loom_Interrupt_Manager::RTC_alarm_exact_from_last_alarm_time(TimeSpan duration)
// {
// 	return (RTC_Inst) ? RTC_alarm_exact(last_alarm_time + duration) : false;
// }

// bool Loom_Interrupt_Manager::RTC_alarm_exact_from_last_alarm_time(uint days, uint hours, uint minutes, uint seconds)
// {
// 	return RTC_alarm_exact_from_last_alarm_time( TimeSpan(days, hours, minutes, seconds) );
// }


/////////////////////////////////////////////////////////////////////
bool Loom_Interrupt_Manager::RTC_alarm_exact(DateTime future_time)
{
	// Don't sleep if no RTC to wake up device
	if (RTC_Inst == NULL) {
		return false;
	}

	print_module_label();
	Println("Will set alarm for : ");
	RTC_Inst->print_DateTime(future_time);


	// Don't set alarm at time in the past
	DateTime now = RTC_Inst->now();
	if ( (future_time - now).totalseconds() < 0 ) {
		print_module_label();
		Println("Wont wake from alarm in the past, increasing time to following day");
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
		Println("Will instead try to set alarm for : ");
		RTC_Inst->print_DateTime(future_time);

	}

	// Tell RTC_Inst to set RTC time at future_time
	// Then call sleep_until_interrupt on pin, because that is what it is
	RTC_Inst->set_alarm(future_time);

}

/////////////////////////////////////////////////////////////////////
bool Loom_Interrupt_Manager::RTC_alarm_exact(uint hour, uint minute, uint second)
{
	// Don't sleep if no RTC to wake up device
	if (RTC_Inst == NULL) {
		return false;
	}

	// Call RTC_alarm_exact(DateTime future_time) with that time today
	// That function will adjust to following day if necessary
	DateTime now = RTC_Inst->now();
	return RTC_alarm_exact( DateTime(now.year(), now.month(), now.day(), hour, minute, second) ); 
}



/////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::register_timer(uint timer_num, unsigned long duration, ISRFuncPtr ISR, bool repeat)
{
	if (timer_num < MaxTimerCount) {
		timer_settings[timer_num] = { ISR, duration, repeat, true };
		timers[timer_num].start(duration, AsyncDelay::MILLIS);
	} else {
		print_module_label();
		Println("Timer number out of range");
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Interrupt_Manager::clear_timer(uint timer_num)
{
	if (timer_num < MaxTimerCount) {
		if (print_verbosity == Verbosity::V_HIGH) {
			print_module_label();
			Println2("Clear timer ", timer_num);
		}
		timers[timer_num].expire();
		timer_settings[timer_num].enabled = false;
	} else {
		print_module_label();
		Println("Timer number out of range");
	}
}

/////////////////////////////////////////////////////////////////////



