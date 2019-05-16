
#ifndef LOOM_SLEEP_MANAGER_h
#define LOOM_SLEEP_MANAGER_h

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

public:

	/// Sleep Manager module constructor.
	///
	/// \param[in]	module_name			String | <"Sleep-Manager"> | null | Sleep Manager module name
	/// \param[in]	use_LED				Bool | <true> | {true, false} | Whether or not to use LED to indicate wake state
	/// \param[in]	delay_on_wake		Bool | <false> | {true, false} | Whether or not to delay upon waking to allow time to open Serial Monitor
	/// \param[in]	sleep_mode			Set(SleepMode) | <1> | { 0("Idle"), 1("Standby"), 2("SleepyDog"), 3("Opens Low Power")} | Which SleepMode to use
	Loom_Sleep_Manager(
			const char*		module_name			= "Sleep_Manager",
			bool			use_LED				= true,
			bool			delay_on_wake		= false,
			SleepMode		sleep_mode			= SleepMode::STANDBY
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

	bool		message_route(OSCMessage& msg, int address_offset) {}

	void 		link_device_manager(LoomManager* LM);



	// /// Link an Interrupt Manager
	// /// \param[in]	IM		Interrupt manager to link
	// void					set_Interrupt_Manager(Loom_Interrupt_Manager* IM);
	// /// Get linked Interrupt Manager
	// /// \return		Linked Interrupt Manager (Null if none linked)
	// Loom_Interrupt_Manager*	get_Interrupt_Manager();


	/// Set the sleep mode to use
	/// \param[in]	mode	The SleepMode to set to
	void		set_sleep_mode(SleepMode mode);
	/// Get the current sleep mode
	/// \return		The current sleep mode
	SleepMode	get_sleep_mode();

	// Standby mode
	// bool sleep_until_interrupt_on(byte pin);
	bool		sleep();

private:

	bool		sleepy_dog_sleep(TimeSpan duration);
	// bool sleepy_dog_sleep(uint days, uint hours, uint minutes, uint seconds);

	void		pre_sleep();
	void		post_sleep();

};


#endif // of #ifndef LOOM_SLEEP_MANAGER_h

