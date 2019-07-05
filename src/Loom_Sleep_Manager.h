#pragma once

#include "Loom_Module.h"
#include "Loom_Interrupt_Manager.h"
#include "Loom_Manager.h"

#include <OPEnS_RTC.h>

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>


class Loom_Interrupt_Manager;

/// Different options available to sleep in
enum class SleepMode {
	IDLE, 			///< Idle
	STANDBY, 		///< Standby
	SLEEPYDOG, 		///< SleepyDog (sleep 'hack')
	OPENS_LOWPOWER	///< OPEnS board to shut board off completely
};


///////////////////////////////////////////////////////////////////////////////


// ### (LoomModule) | dependencies: [] | conflicts: []
/// Submanager to manage sleep functionality
// ###
class Loom_Sleep_Manager : public LoomModule
{

protected:
	
	Loom_Interrupt_Manager* interrupt_manager;	/// Pointer to interrupt_manager instance 
	
	bool 		use_LED;			/// Whether or not to use LED to indicate wake status
	bool		delay_on_wake;		/// Whether to provide delay on wake.
									/// Used to allow user to restart Serial Monitor
	SleepMode 	sleep_mode;			/// Which sleep mode to use
	byte		power_off_pin;		///	Which pin to use to power board off (requires power board)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Sleep Manager module constructor.
	///
	/// \param[in]	module_name			String | <"Sleep-Manager"> | null | Sleep Manager module name
	/// \param[in]	use_LED				Bool | <true> | {true, false} | Whether or not to use LED to indicate wake state
	/// \param[in]	delay_on_wake		Bool | <false> | {true, false} | Whether or not to delay upon waking to allow time to open Serial Monitor
	/// \param[in]	sleep_mode			Set(SleepMode) | <1> | { 0("Idle"), 1("Standby"), 2("SleepyDog"), 3("Opens Low Power")} | Which SleepMode to use
	/// \param[in]	power_off_pin		Set(Int) | <10> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin should be used to power off board
	Loom_Sleep_Manager(
			const char*		module_name			= "Sleep_Manager",
			bool			use_LED				= true,
			bool			delay_on_wake		= false,
			SleepMode		sleep_mode			= SleepMode::STANDBY,
			byte			power_off_pin		= A5
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_Sleep_Manager(JsonArrayConst p);

	//// Destructor
	~Loom_Sleep_Manager() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void 		package(JsonObject json) override {}
	bool		cmd_route(JsonObject) override {}

	/// Put into low power state.
	/// On wake, program will continue from where it went to sleep
	/// \return Whether or not sleep was successful
	bool		sleep();

	/// Turn board off.
	/// Program will restart from setup on wake
	void		powerDown();

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_state() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the current sleep mode
	/// \return		The current sleep mode
	SleepMode	get_sleep_mode();

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set pointer to interrupt manager
	/// \param[in]	IM	Pointer to an interrupt manager	
	void 		link_interrupt_manager(Loom_Interrupt_Manager* IM);

	/// Set the sleep mode to use
	/// \param[in]	mode	The SleepMode to set to
	void		set_sleep_mode(SleepMode mode);

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	/// Convert enum of sleep mode to a c-string
	/// \param[in]	m	Sleep to get string of
	/// \return C-string of sleep mode
	const static char* enum_sleep_mode_string(SleepMode m);
	
private:

	/// Handles pre-sleep operations
	void		pre_sleep();

	/// Handles post-sleep operations
	void		post_sleep();
};



