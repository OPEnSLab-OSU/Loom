#pragma once

#include "Loom_Module.h"

#include <OPEnS_RTC.h>

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>



/// Different options available to sleep in
enum class SleepMode {
	IDLE, 			///< Idle
	STANDBY, 		///< Standby
	SLEEPYDOG, 		///< SleepyDog (sleep 'hack')
	OPENS_LOWPOWER	///< OPEnS board to shut board off completely
};


// ### (LoomModule) | dependencies: [] | conflicts: []
/// Submanager to manage sleep functionality
// ###
class Loom_Sleep_Manager : public LoomModule
{

protected:

	/// Whether or not to use LED to indicate wake status
	bool 		use_LED;
	/// Whether to provide delay on wake.
	/// Used to allow user to restart Serial Monitor
	bool		delay_on_wake;

	/// Which sleep mode to use
	SleepMode 	sleep_mode;

	///	Which pin to use to power board off (requires power board)
	byte		power_off_pin;

public:

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

	Loom_Sleep_Manager(JsonVariant p);

	const static char* enum_sleep_mode_string(SleepMode m);


	// --- DESTRUCTOR ---
	virtual ~Loom_Sleep_Manager();

	// Inherited methods
	void		print_config() override;
	void		print_state() override;
	void		measure() {}
	void 		package(JsonObject json) {}
	bool		cmd_route(JsonObject) {}


	// Put into low power state.
	// On wake, program will continue from where it went to sleep
	bool		sleep();
	// Turn board off.
	// Program will restart from setup on wake
	void		powerDown();




	/// Set the sleep mode to use
	/// \param[in]	mode	The SleepMode to set to
	void		set_sleep_mode(SleepMode mode);
	/// Get the current sleep mode
	/// \return		The current sleep mode
	SleepMode	get_sleep_mode();

private:

	bool		sleepy_dog_sleep(TimeSpan duration);
	// bool sleepy_dog_sleep(uint days, uint hours, uint minutes, uint seconds);

	void		pre_sleep();
	void		post_sleep();

};



