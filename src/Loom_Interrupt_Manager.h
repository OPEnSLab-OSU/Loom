#pragma once

#include "Loom_Module.h"


#include <OPEnS_RTC.h>
#include <AsyncDelay.h>


// #include <initializer_list>



/// Number of interrupts
#define InteruptRange 16

/// Maximum number of timers
#define MaxTimerCount 4
/// Maximum numbr of stopwatches
#define MaxStopWatchCount 2




// Specify that LoomRTC exists, defined in own file
class LoomRTC;


// Used to make function signatures easier to read
// when returning function pointers
/// Typedef to for ISR function pointer readability
using ISRFuncPtr = void (*)();
// using ISRFuncPtr = void *();


enum class ISR_Type { IMMEDIATE, CHECK_FLAG };



/// Contains information defining an interrupt's configuration
struct IntDetails {
	ISRFuncPtr	ISR;			///< Function pointer to ISR. Set null if no interrupt linked
	byte		type;			///< Interrupt signal type to detect. LOW: 0, HIGH: 1, CHANGE: 2, FALLING: 3, INT_RISING: 4
	ISR_Type	run_type;		///< True if ISR is called directly upon interrupt, false if called next check of flags
	bool		enabled;		///< Whether or not this interrupt is enabled
};

/// Contains information defining a timer's configuration
struct TimerDetails {
	ISRFuncPtr	ISR;			///< Not a real ISR, just a function called if timer has expired
	uint		duration;		///< The timer duration
	bool		repeat;			///< Whether or not timer should repeat
	bool		enabled;		///< Whether or not this timer is enabled
};

/// Contains information defining a stopwatch's configuration
struct StopWatchDetails {
	unsigned long	start_time;		///< The millis time when stopwatch started
	bool			enabled;		///< Whether or not this stopwatch is enabled
};



// ### (LoomModule) | dependencies: [] | conflicts: []
/// Submanager for managing interrupts, timers, and stopwatches
// ###
class Loom_Interrupt_Manager : public LoomModule
{

protected:

	/// Enable or disable all interrupts 	-- currently only disables bottom halves
	bool			interrupts_enabled;

	/// List of interrupts configurations
	IntDetails		int_settings[InteruptRange];

	// Flags set by interrupts, indicating ISR bottom
	// half should be called if not Null
	static bool 	interrupt_triggered[InteruptRange];

	/// Pointer to an RTC object for managing timers / timed interrupts
	LoomRTC*		RTC_Inst;

	/// Pointer to a Sleep Manager object
	Loom_Sleep_Manager* Sleep_Manager;

	/// Last time an alarm went off
	DateTime		last_alarm_time;


// millis timers
	AsyncDelay		timers[MaxTimerCount];
	TimerDetails	timer_settings[MaxTimerCount];


	StopWatchDetails	stopwatch_settings[MaxStopWatchCount];


	// interrupt_triggered for timers, also support immediate and delayed
public:


	/// Interrupt Manager module constructor.
	///
	/// \param[in]	module_name		String | <"Interrupt-Manager"> | null | Interrupt Manager module name
	/// \param[in]	RTC_Inst			Set(Int) | <0> | {0("Null")} | OLED module name
	Loom_Interrupt_Manager(
			const char*		module_name		= "Interrupt_Manager",
			LoomRTC*		RTC_Inst		= nullptr
		);

	Loom_Interrupt_Manager(JsonVariant p);


	// --- DESTRUCTOR ---
	virtual ~Loom_Interrupt_Manager();


	const static char* interrupt_type_to_string(int type);


	// Inherited methods
	void		print_config() override;
	void		print_state() override;
	void		measure() {}
	void 		package(JsonObject json) {}
	bool		cmd_route(JsonObject) {}


	void 		link_device_manager(LoomManager* LM);
	void 		link_sleep_manager(Loom_Sleep_Manager* SM);

	void		run_pending_ISRs();


// === === Interrupt Functions === ===


	/// All interrupts enable/disable
	/// \param[in]	state	Enable state to apply to all interrupts
	void		set_interrupts_enabled(bool state);
	/// Get global interrupt enable state
	/// \return		Global interrupt enable state
	bool		get_interrupts_enabled();

	/// Per interrupt enable
	/// \param[in]	pin		Interrupt pin to change enable state of
	/// \param[in]	state	The enable state to set pin to
	void		set_enable_interrupt(byte pin, bool state);
	/// Get pin interrupt enable state
	/// \return		The enable state
	bool		get_enable_interrupt(byte pin);


	/// Register an ISR to an interrupt pin and its configuration
	/// \param[in]	pin			Which pin to connect the interrupt on
	/// \param[in]	ISR			ISR	function (Null if no interrupt linked)
	/// \param[in]	type		Low, High, Change, Falling, Rising
	/// \param[in]	immediate	Whether the interrupt runs immediately, else sets flag to check and runs ISR when flag checked
	void		register_ISR(byte pin, ISRFuncPtr ISR, byte signal_type, ISR_Type run_type);


	/// Restores pin to default ISR, disables interrupt
	/// \param[in]	pin		The pin to unregister ISRs for
	/// \param[in]	type	What signal to configure default ISR to (default LOW)
	void		unregister_ISR(byte pin, byte signal_type=LOW);

	/// Detaches then reattacheds interrupt according to settings.
	/// used to clear pending interrupts
	/// \param[in]	pin 	Pin to reset interrupts for ...
	void		interrupt_reset(byte pin);



// === === RTC Alarm Functions === ===

	/// Set the RTC module to use for timers
	/// \param[in]	RTC_Inst	Pointer to the RTC object
	void		set_RTC_module(LoomRTC* RTC_Inst);
	/// Return pointer to the currently linked RTC object
	/// \return		Current RTC object
	LoomRTC*	get_RTC_module();



// Shorten names, maybe combine, taking behavior (e.g. relative/absolute) as parameter



// sleep_for
	/// Set RTC alarm relative time from now
	/// \param[in]	duration	How long before the alarm should go off
	bool		RTC_alarm_duration(TimeSpan duration);

	/// Set RTC alarm relative time from now
	/// \param[in]	days		Days into the future the alarm should be set
	/// \param[in]	hours		Hours into the future the alarm should be set
	/// \param[in]	minutes		Minutes into the future the alarm should be set
	/// \param[in]	seconds		Seconds into the future the alarm should be set
	bool		RTC_alarm_duration(uint days, uint hours, uint minutes, uint seconds);

// sleep_until
	/// Set RTC alarm for a specific time.
	/// Increments to next day at given hour, min, sec if specified time is in past
	/// \param[in]	future_time		Time to set alarm for
	bool		RTC_alarm_at(DateTime future_time);

	/// Set RTC alarm for a specific time.
	/// Forwards to RTC_alarm_at that takes DateTime object
	/// \param[in]	hour		Hour to set alarm for
	/// \param[in]	minute		Minute to set alarm for
	/// \param[in]	second		Second to set alarm for
	bool		RTC_alarm_at(uint hour, uint minute, uint second);



// // sleep_for
	/// Set RTC alarm relative time from last alarm time
	/// \param[in]	duration	How long before the alarm should go off
	bool		RTC_alarm_duration_from_last(TimeSpan duration);

	/// Set RTC alarm relative time from last alarm time
	/// \param[in]	days		Days into the future the alarm should be set
	/// \param[in]	hours		Hours into the future the alarm should be set
	/// \param[in]	minutes		Minutes into the future the alarm should be set
	/// \param[in]	seconds		Seconds into the future the alarm should be set
	bool		RTC_alarm_duration_from_last(uint days, uint hours, uint minutes, uint seconds);



// some sort of alarm auto repeating


// === === AsyncDelay Timer Functions === ===


	/// Check if timers have elapsed, if so run associated 'ISR'
	void		check_timers();

	/// Configure specified timer
	void		register_timer(uint timer_num, unsigned long duration, ISRFuncPtr ISR, bool repeat);

	/// Clear specified timer
	void		clear_timer(uint timer_num);




// === === Stopwatch Functions === ===




// === === Internal Timer Functions === ===


	// https://github.com/GabrielNotman/RTCCounter


private:

	/// Checks the flags set by default ISRs, calls pending bottom half ISRs
	void		run_ISR_bottom_halves();


	// Default ISRs that set flags, detach interrupt to prevent multiple triggering (reattached after checking flag)
	static void default_ISR_0()  { interrupt_triggered[0]  = true; };
	static void default_ISR_1()  { interrupt_triggered[1]  = true; };
	static void default_ISR_2()  { interrupt_triggered[2]  = true; };
	static void default_ISR_3()  { interrupt_triggered[3]  = true; };
	static void default_ISR_4()  { interrupt_triggered[4]  = true; };
	static void default_ISR_5()  { interrupt_triggered[5]  = true; };
	static void default_ISR_6()  { interrupt_triggered[6]  = true; };
	static void default_ISR_7()  { interrupt_triggered[7]  = true; };
	static void default_ISR_8()  { interrupt_triggered[8]  = true; };
	static void default_ISR_9()  { interrupt_triggered[9]  = true; };
	static void default_ISR_10() { interrupt_triggered[10] = true; };
	static void default_ISR_11() { interrupt_triggered[11] = true; };
	static void default_ISR_12() { interrupt_triggered[12] = true; };
	static void default_ISR_13() { interrupt_triggered[13] = true; };
	static void default_ISR_14() { interrupt_triggered[14] = true; };
	static void default_ISR_15() { interrupt_triggered[15] = true; };

// detaching interrupt did not seem to work
	// static void default_ISR_0()  { detachInterrupt(digitalPinToInterrupt(0));  interrupt_triggered[0]  = true;   }


	/// Array of the default ISRs that set flags
	const static ISRFuncPtr default_ISRs[InteruptRange];

};


