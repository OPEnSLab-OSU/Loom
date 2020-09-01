///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Sleep_Manager.h
/// @brief		File for Loom_Sleep_Manager definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Module.h"
#include "Interrupt_Manager.h"
#include "Manager.h"

#include <OPEnS_RTC.h>

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>


class Loom_Interrupt_Manager;


///////////////////////////////////////////////////////////////////////////////
///
/// Submanager to manage sleep functionality.
///
/// @par Resources
/// - [Documentation](https://openslab-osu.github.io/Loom/html/class_loom___sleep___manager.html)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_Sleep_Manager : public LoomModule
{

public:

	/// Different options available to sleep in
	enum class Mode {
		IDLE, 			///< Idle
		STANDBY, 		///< Standby
		OPENS_LOWPOWER	///< OPEnS board to shut board off completely
	};

protected:

	Loom_Interrupt_Manager* interrupt_manager;	///< Pointer to interrupt_manager instance

	bool 		use_LED;			///< Whether or not to use LED to indicate wake status
	bool		delay_on_wake;		///< Whether to provide delay on wake.
									///< Used to allow user to restart Serial Monitor
	Mode 		sleep_mode;			///< Which sleep mode to use
	byte		power_off_pin;		///< Which pin to use to power board off (requires power board)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Sleep Manager module constructor.
	///
	/// @param[in]	use_LED				Bool | <true> | {true, false} | Whether or not to use LED to indicate wake state
	/// @param[in]	delay_on_wake		Bool | <false> | {true, false} | Whether or not to delay upon waking to allow time to open Serial Monitor
	/// @param[in]	sleep_mode			Set(Mode) | <1> | { 0("Idle"), 1("Standby"), 2("Opens Low Power")} | Which Mode to use
	/// @param[in]	power_off_pin		Set(Int) | <10> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin should be used to power off board
	Loom_Sleep_Manager(
			LoomManager* manager,
			const bool		use_LED				= true,
			const bool		delay_on_wake		= false,
			const Mode		sleep_mode			= Mode::STANDBY,
			const byte		power_off_pin		= A5
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_Sleep_Manager(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_Sleep_Manager() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// No package necessary.
	/// Implement with empty body.
	void		package(JsonObject json) override {}

	/// Put into low power state.
	/// On wake, program will continue from where it went to sleep
	/// @return Whether or not sleep was successful
	bool		sleep();

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the current sleep mode
	/// @return		The current sleep mode
	Mode		get_sleep_mode() const { return sleep_mode; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set pointer to interrupt manager
	/// @param[in]	IM	Pointer to an interrupt manager
	void 		link_interrupt_manager(Loom_Interrupt_Manager* IM) { interrupt_manager = IM; }

	/// Set the sleep mode to use
	/// @param[in]	mode	The Mode to set to
	void		set_sleep_mode(const Mode mode) { sleep_mode = mode; }

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	void 		link_device_manager(LoomManager* LM) override;

	/// Convert enum of sleep mode to a c-string
	/// @param[in]	m	Sleep to get string of
	/// @return C-string of sleep mode
	const static char* enum_sleep_mode_string(const Mode m);

private:

	/// Handles pre-sleep operations
	void		pre_sleep();

	/// Handles post-sleep operations
	void		post_sleep();
};
