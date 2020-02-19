///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Interrupt_Manager.h
/// @brief		File for Loom_Interrupt_Manager definition and supporting enum
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Module.h"

#include <OPEnS_RTC.h>
#include <AsyncDelay.h>
#include <RTCCounter.h>


#define InteruptRange 16		///< Number of interrupts
#define MaxTimerCount 2			///< Maximum number of timers
#define MaxStopWatchCount 2		///< Maximum numbr of stopwatches


// Specify that LoomRTC exists, defined in own file
class LoomRTC;
class Loom_Sleep_Manager;


/// Used to make function signatures easier to read
/// when returning function pointers
using ISRFuncPtr = void (*)();

/// Enum of different ISR run behavior
enum class ISR_Type { 
	IMMEDIATE, 		///< Run ISR immediately
	CHECK_FLAG		///< Run ISR when run_pending_ISRs() is called
};


///////////////////////////////////////////////////////////////////////////////
///
/// Submanager for managing interrupts, timers, and stopwatches.
///
/// @par Resources
/// - [Documentation](https://openslab-osu.github.io/Loom/html/class_loom___interrupt___manager.html)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_Interrupt_Manager : public LoomModule
{

private:

	/// Contains information defining an interrupt's configuration
	struct IntDetails {
		ISRFuncPtr	ISR;			///< Function pointer to ISR. Set null if no interrupt linked
		uint32_t		type;			///< Interrupt signal type to detect. LOW: 0, HIGH: 1, CHANGE: 2, FALLING: 3, INT_RISING: 4
		ISR_Type	run_type;		///< True if ISR is called directly upon interrupt, false if called next check of flags
		bool		enabled;		///< Whether or not this interrupt is enabled
	};

	/// Contains information defining a timer's configuration
	struct InternalTimerDetails {
		ISRFuncPtr	ISR;			///< Function pointer to ISR. Set null if no interrupt linked
		ISR_Type	run_type;		///< True if ISR is called directly upon interrupt, false if called next check of flags
		uint		duration;		///< The timer duration
		bool		repeat;			///< Whether or not timer should repeat
		bool		enabled;		///< Whether or not this timer is enabled
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

protected:

	/// Pointer to an RTC object for managing timers / timed interrupts	
	LoomRTC*		RTC_Inst;					
	
	/// Pointer to a Sleep Manager object
	Loom_Sleep_Manager* Sleep_Manager;	

	// = = = Interrupts = = =

	/// List of interrupts configurations
	IntDetails		int_settings[InteruptRange];		
	
	/// Flags set by interrupts, indicating ISR bottom
	/// half should be called if not Null
	static bool 	interrupt_triggered[InteruptRange];

	/// Enable or disable all interrupts 	-- currently only disables bottom halves
	bool			interrupts_enabled;			
	
	// = = = Interal Timer = = =

	InternalTimerDetails	internal_timer;

	// = = = Timers = = =

	/// Last time an alarm went off
	DateTime		last_alarm_time;			

	// millis timers
	AsyncDelay		timers[MaxTimerCount];
	TimerDetails	timer_settings[MaxTimerCount];

	// = = = Stopwatches = = =

	StopWatchDetails	stopwatch_settings[MaxStopWatchCount];


	// interrupt_triggered equivalent for timers, also support immediate and delayed
public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Interrupt Manager module constructor.
	///
	/// @param[in]	RTC_Inst		Set(Int) | <0> | {0("Null")} | OLED module name
	Loom_Interrupt_Manager(
			LoomManager* manager,
			LoomRTC*		RTC_Inst		= nullptr
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_Interrupt_Manager(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_Interrupt_Manager() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// No package necessary.
	/// Implement with empty body.
	void		package(JsonObject json) override {}

	/// Run any waiting ISRs.
	/// Flag was set by a top half ISR
	void		run_pending_ISRs();

//=============================================================================
///@name	EXTERNAL INTERRUPT METHODS
/*@{*/ //======================================================================

	/// Register an ISR to an interrupt pin and its configuration
	/// @param[in]	pin			Which pin to connect the interrupt on
	/// @param[in]	ISR			ISR	function (Null if no interrupt linked)
	/// @param[in]	signal_type	Low, High, Change, Falling, Rising
	/// @param[in]	run_type	Whether the interrupt runs immediately, else sets flag to check and runs ISR when flag checked
	void		register_ISR(const uint32_t pin, ISRFuncPtr ISR, const uint32_t signal_type, ISR_Type run_type);

	/// If an ISR dettaches interrupt, use this to reattach according to 
	/// previous settings.
	/// Effectively to running register_ISR with previous settings
	/// @param[in]	pin			Which pin to reconnect the interrupt on
	bool		reconnect_interrupt(const uint32_t pin);


	/// Restores pin to default ISR, disables interrupt
	/// @param[in]	pin				The pin to unregister ISRs for
	/// @param[in]	signal_type		What signal to configure default ISR to (default LOW)
	void		unregister_ISR(const uint32_t pin, const uint32_t signal_type=LOW);

	/// Detaches then reattacheds interrupt according to settings.
	/// used to clear pending interrupts
	/// @param[in]	pin 	Pin to reset interrupts for ...
	void		interrupt_reset(const uint32_t pin);

//=============================================================================
///@name	RTC ALARM METHODS
/*@{*/ //======================================================================

	// Shorten names, maybe combine, taking behavior (e.g. relative/absolute) as parameter

	// sleep_for
	/// Set RTC alarm an amount of time from now
	/// @param[in]	duration	How long before the alarm should go off
	bool		RTC_alarm_duration(const TimeSpan duration);

	/// Set RTC alarm an amount of time from now
	/// @param[in]	days		Days into the future the alarm should be set
	/// @param[in]	hours		Hours into the future the alarm should be set
	/// @param[in]	minutes		Minutes into the future the alarm should be set
	/// @param[in]	seconds		Seconds into the future the alarm should be set
	bool		RTC_alarm_duration(const uint8_t days, const uint8_t hours, const uint8_t minutes, const uint8_t seconds);

	// sleep_until
	/// Set RTC alarm for a specific time.
	/// Increments to next day at given hour, min, sec if specified time is in past
	/// @param[in]	future_time		Time to set alarm for
	bool		RTC_alarm_at(DateTime future_time);

	/// Set RTC alarm for a specific time.
	/// Forwards to RTC_alarm_at that takes DateTime object
	/// @param[in]	hour		Hour to set alarm for
	/// @param[in]	minute		Minute to set alarm for
	/// @param[in]	second		Second to set alarm for
	bool		RTC_alarm_at(const uint8_t hour, const uint8_t minute, const uint8_t second);

// // sleep_for
	/// Set RTC alarm an amount of time from last alarm time
	/// @param[in]	duration	How long before the alarm should go off
	bool		RTC_alarm_duration_from_last(const TimeSpan duration);

	/// Set RTC alarm an amount of time from last alarm time
	/// @param[in]	days		Days into the future the alarm should be set
	/// @param[in]	hours		Hours into the future the alarm should be set
	/// @param[in]	minutes		Minutes into the future the alarm should be set
	/// @param[in]	seconds		Seconds into the future the alarm should be set
	bool		RTC_alarm_duration_from_last(const uint8_t days, const uint8_t hours, const uint8_t minutes, const uint8_t seconds);


//=============================================================================
///@name	ASYNCDELAY TIMER METHODS
/*@{*/ //======================================================================

// maybe remove in favor of internal timers

	/// Check if timers have elapsed, if so run associated 'ISR'
	void		check_timers();

	/// Configure specified timer
	/// @param[in]	timer_num		Timer to set
	/// @param[in]	duration		How long timer should take (seconds)
	/// @param[in]	ISR				ISR to run after timer goes off
	/// @param[in]	repeat			Whether or not to be a repeating alarm
	void		register_timer(const uint8_t timer_num, const unsigned long duration, const ISRFuncPtr ISR, const bool repeat);

	/// Clear specified timer 
	/// @param[in]	timer_num		Timer to clear
	void		clear_timer(const uint8_t timer_num);

//=============================================================================
///@name	STOPWATCH METHODS
/*@{*/ //======================================================================


//=============================================================================
///@name	INTERNAL TIMER METHODS
/*@{*/ //======================================================================

	// uses https://github.com/GabrielNotman/RTCCounter

	/// Configure internal timer
	/// @param[in]	duration		How long timer should take (seconds)
	/// @param[in]	ISR				ISR to run after timer goes off
	/// @param[in]	repeat			Whether or not to be a repeating alarm
	/// @param[in]	run_type	Whether the interrupt runs immediately, else sets flag to check and runs ISR when flag checked
	void		register_internal_timer(const uint duration, const ISRFuncPtr ISR, const bool repeat, const ISR_Type run_type);		

	/// Run a delayed (flag based) ISR if the interal timer elapsed.
	/// Is not needed if using ISR_Type::IMMEDIATE ISR
	/// @return	True if flag was set and ISR run
	bool		run_pending_internal_timer_ISR();

	/// Get whether the internal timer has elapsed
	/// @return True if timer elapsed, false otherwise
	bool		get_internal_timer_flag() const { return rtcCounter.getFlag(); }

	/// Clear internal timer flag
	void		clear_internal_timer_flag() const { rtcCounter.clearFlag(); }

	/// Enable or disable internal timer.
	/// Disabling does not remove settings, use unregister_interal_timer for that behavior
	/// @param[in]	enable	True to enable timer, false to disable
	void		internal_timer_enable(const bool enable);

	/// Clear and disable internal timer, remove ISR
	void		unregister_internal_timer();

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;
	void		print_state() const override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get global interrupt enable state
	/// @return		Global interrupt enable state
	bool		get_interrupts_enabled() const { return interrupts_enabled; }

	/// Get pin interrupt enable state
	/// @param[in]	pin		Pin to the the enable state of
	/// @return		The enable state
	bool		get_enable_interrupt(const uint32_t pin) const { return (pin < InteruptRange) ? int_settings[pin].enabled : false; }

	/// Return pointer to the currently linked RTC object
	/// @return		Current RTC object
	LoomRTC*	get_RTC_module() const { return RTC_Inst; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Link a device manager.
	/// Overrides default by getting RTC pointer from 
	/// device manager if possible
	/// @param[in]	LM		Manager to link 
	void 		link_device_manager(LoomManager* LM) override;

	/// Set pointer to sleep Manager
	/// @param[in]	SM		Pointer to sleep manager
	void 		link_sleep_manager(Loom_Sleep_Manager* SM);

	/// All interrupts enable/disable
	/// @param[in]	state	Enable state to apply to all interrupts
	void		set_interrupts_enabled(const bool state) { interrupts_enabled = state; }

	/// Per interrupt enable
	/// @param[in]	pin		Interrupt pin to change enable state of
	/// @param[in]	state	The enable state to set pin to
	void		set_enable_interrupt(const uint32_t pin, const bool state);

	/// Set the RTC module to use for timers
	/// @param[in]	RTC_Inst	Pointer to the RTC object
	void		set_RTC_module(LoomRTC* RTC_Inst) { this->RTC_Inst = RTC_Inst; }

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	/// Get c-string of name associated with interrupt type enum
	/// @param[in]	type	Interrupt type 
	/// @return C-string of interrupt type
	const static char* interrupt_type_to_string(const uint8_t type);


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


