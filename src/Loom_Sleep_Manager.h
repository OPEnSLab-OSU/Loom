
#ifndef LOOM_SLEEP_MANAGER_h
#define LOOM_SLEEP_MANAGER_h

#include "Loom_Module.h"


// #include <RTClibExtended.h>
// // NOTE: Must include the following line in the RTClibExtended.h file to use with M0:
// // #define _BV(bit) (1 << (bit))

#include <OPEnS_RTC.h>

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>


class LoomRTC; // Specify that LoomRTC exists, defined in own file
// class LoomManager; // Specify that LoomManager exists, defined in own file



enum class SleepMode { 
	IDLE, 			///< Idle
	STANDBY, 		///< Standby
	SLEEPYDOG, 		///< SleepyDog (sleep 'hack')
	OPENS_LOWPOWER	///< OPEnS board to shut board off completely
};




class Loom_Sleep_Manager : public LoomModule
{

protected:

	/// Whether or not to use LED to indicate wake status
	bool 		use_LED;
	/// Whether to provide delay on wake.
	/// Used to allow user to restart Serial Monitor
	bool		delay_on_wake;

	/// Keep track of last time device awoke.
	/// Used to prevent drift in setting next alarm
	DateTime 	last_wake_time;

	/// Pointer to Interrupt Manager.
	/// Offload timers and interrupts to interrupts manager
	Loom_Interrupt_Manager* IM;
	/// Pointer to RTC object
	// Might remove this pointer and only leave in Interrupt Manager
	LoomRTC* 	RTC_Inst;

	/// Which sleep mode to use
	SleepMode 	sleep_mode;

public:

	// --- CONSTRUCTOR ---
	Loom_Sleep_Manager( 
			char*		module_name			= "Sleep_Manager",
				
			LoomRTC*	RTC_Inst			= NULL,

			bool		use_LED				= true,
			bool		delay_on_wake		= false,

			SleepMode	sleep_mode			= SleepMode::STANDBY

		);

	// Loom_Sleep_Manager( char* module_name, LoomManager* LD );

	const static char* enum_sleep_mode_string(SleepMode m);


	// --- DESTRUCTOR ---
	~Loom_Sleep_Manager();

	// Inherited methods
	void		print_config();
	void		print_state();
	void		measure() {}
	void		package(OSCBundle& bndl, char* suffix="") {}
	bool		message_route(OSCMessage& msg, int address_offset) {}

	void 		link_device_manager(LoomManager* LM);


	/// Set the RTC module to use for timers
	/// \param[in]	RTC_Inst	Pointer to the RTC object
	void		set_RTC_module(LoomRTC* RTC_Inst);
	/// Return pointer to the currently linked RTC object
	/// \return		Current RTC object
	LoomRTC*	get_RTC_module();


	/// Link an Interrupt Manager
	/// \param[in]	IM		Interrupt manager to link
	void		set_Interrupt_Manager(Loom_Interrupt_Manager* IM);
	/// Get linked Interrupt Manager
	/// \return		Linked Interrupt Manager (Null if none linked)
	Loom_Interrupt_Manager* get_Interrupt_Manager();


	/// Set the sleep mode to use
	/// \param[in]	mode	The SleepMode to set to
	void		set_sleep_mode(SleepMode mode);
	/// Get the current sleep mode
	/// \return		The current sleep mode
	SleepMode	get_sleep_mode();


// maybe offload setting timer to interrupt manager

// extract timer from being paired with sleep


// Keep sleep time from wake controlled here


	bool		sleep_duration(TimeSpan duration);	
	bool		sleep_duration(uint days, uint hours, uint minutes, uint seconds);
	
	// Only works with RTC
	// Might work using PCF and SleepyDog together, otherwise has to be DS3231
	bool		sleep_duration_from_wake(TimeSpan duration);
	bool		sleep_duration_from_wake(uint days, uint hours, uint minutes, uint seconds);
	
	// Only works with RTC
	// Might work using PCF and SleepyDog together, otherwise has to be DS3231
	bool		sleep_until_time(DateTime future_time);
	bool		sleep_until_time(uint hour, uint minute, uint second);
	
	// Standby mode
	// bool sleep_until_interrupt_on(byte pin);
	bool		sleep();


	// Some sort of auto repeat? - probably manage that at DeviceManager level

private:

	bool		sleepy_dog_sleep(TimeSpan duration);
	// bool sleepy_dog_sleep(uint days, uint hours, uint minutes, uint seconds);

	void		pre_sleep();
	void		post_sleep();




// Probably not needed:
	// Wrappers for the other (pre/post)sleep but handles RTC alarms

	// Unless the RTC alarm resetting is part of the interrupt manager not sleep manager
	// Or in virtual LoomModule sleep/wake() methods
	// Resetting might not be needed until next call to sleep_duration_from_wake
	// void RTC_pre_sleep();
	// void RTC_post_sleep(); 

};


#endif // of #ifndef LOOM_SLEEP_MANAGER_h










