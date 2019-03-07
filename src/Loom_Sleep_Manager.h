
#ifndef LOOM_SLEEP_MANAGER_h
#define LOOM_SLEEP_MANAGER_h

#include "Loom_Module.h"


#include <RTClibExtended.h>
// NOTE: Must include the following line in the RTClibExtended.h file to use with M0:
// #define _BV(bit) (1 << (bit))

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>


class LoomRTC; // Specify that LoomRTC exists, defined in own file
// class LoomManager; // Specify that LoomManager exists, defined in own file



enum class SleepMode { IDLE, STANDBY, SLEEPYDOG };




class Loom_Sleep_Manager : public LoomModule
{

protected:

	bool 		use_LED;
	bool		delay_on_wake;

	DateTime 	last_wake_time;


	Loom_Interrupt_Manager* IM;
	LoomRTC* 	RTC_Inst;

	SleepMode 	sleep_mode;

public:

	// --- CONSTRUCTOR ---
	Loom_Sleep_Manager( char* 		module_name 	= "Sleep_Manager",
							
						LoomRTC* 	RTC_Inst 		= NULL,

						bool 		use_LED 		= true,
						bool		delay_on_wake 	= false,

						SleepMode 	sleep_mode 		= SleepMode::STANDBY

					);

	// Loom_Sleep_Manager( char* module_name, LoomManager* LD );

	const static char* enum_sleep_mode_string(SleepMode m);


	// --- DESTRUCTOR ---
	~Loom_Sleep_Manager();

	void print_config();
	void print_state();
	void measure() {}
	void package(OSCBundle& bndl, char* suffix="") {}
	bool message_route(OSCMessage& msg, int address_offset) {}


	void set_RTC_module(LoomRTC* RTC_Inst);
	LoomRTC* get_RTC_module();

	void set_Interrupt_Manager(Loom_Interrupt_Manager* IM);
	Loom_Interrupt_Manager* get_Interrupt_Manager();



	void set_sleep_mode(SleepMode mode);
	SleepMode get_sleep_mode();


// maybe offload setting timer to interrupt manager

// extract timer from being paired with sleep


// Keep sleep time from wake controlled here

	bool sleep_for_time(TimeSpan duration);	
	bool sleep_for_time(uint days, uint hours, uint minutes, uint seconds);
	
	// Only works with RTC
	// Might work using PCF and SleepyDog together, otherwise has to be DS3231
	bool sleep_for_time_from_wake(TimeSpan duration);
	bool sleep_for_time_from_wake(uint days, uint hours, uint minutes, uint seconds);
	
	// Only works with RTC
	// Might work using PCF and SleepyDog together, otherwise has to be DS3231
	bool sleep_until_time(DateTime future_time);
	bool sleep_until_time(uint hour, uint minute, uint second);
	
	// Standby mode
	// bool sleep_until_interrupt_on(byte pin);
	bool sleep();


	// Some sort of auto repeat? - probably manage that at DeviceManager level

private:

	bool sleepy_dog_sleep(TimeSpan duration);
	// bool sleepy_dog_sleep(uint days, uint hours, uint minutes, uint seconds);

	void pre_sleep();
	void post_sleep();




// Probably not needed:
	// Wrappers for the other (pre/post)sleep but handles RTC alarms

	// Unless the RTC alarm resetting is part of the interrupt manager not sleep manager
	// Or in virtual LoomModule sleep/wake() methods
	// Resetting might not be needed until next call to sleep_for_time_from_wake
	// void RTC_pre_sleep();
	// void RTC_post_sleep(); 

};


#endif // of #ifndef LOOM_SLEEP_MANAGER_h










