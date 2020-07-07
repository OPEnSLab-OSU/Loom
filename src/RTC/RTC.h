///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_RTC.h
/// @brief		File for LoomRTC definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Module.h"

#include <OPEnS_RTC.h>


///////////////////////////////////////////////////////////////////////////////
///
/// Abstract base class of RTC modules.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_r_t_c.html)
/// - [Dependency: OPEnS_RTC](https://github.com/OPEnSLab-OSU/OPEnS_RTC)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#data-logging)
///
///////////////////////////////////////////////////////////////////////////////
class LoomRTC : public LoomModule
{

public:

	/// Different time zones
	enum class TimeZone { 
		WAT = 0, AT, ADT, AST, EDT, EST, CDT, CST, MDT, MST, PDT, PST, ALDT, 
		ALST, HST, SST, GMT, BST, CET, CEST, EET, EEST, BT, ZP4, ZP5, 
		ZP6, ZP7, AWST, AWDT, ACST, ACDT, AEST, AEDT 
	};

private: 
	
	const static char*	daysOfTheWeek[];		///< Array of strings the days of the week
	const static float	timezone_adjustment[];	///< Timezone hour adjustment associated with each TimeZone enum

protected:

	TimeZone	timezone;			///< The TimeZone to use

	bool		use_utc_time;		///< Whether or not use UTC time, else local time

	char		datestring[20];		///< Latest saved string of the Date (year/month/day)
	char		timestring[20];		///< Latest saved string of the time (hour:minute:second)

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// @param[in]	module_name		Name of the module (provided by derived classes)
	/// @param[in]	module_type		Type of the module (provided by derived classes)
	/// @param[in]	timezone		Which timezone device is in
	/// @param[in]	use_utc_time	True for UTC time, false for local time
	LoomRTC(
			LoomManager* manager,
			const char*				module_name,
			const LoomModule::Type	module_type,
			const TimeZone			timezone,
			const bool				use_utc_time
		);

	/// Destructor
	virtual ~LoomRTC() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// Adds a timestamp to the provided data Json
	/// @param[out]	json	Object to add timestamp to
	virtual void 	package(JsonObject json) override;

	/// Get DateTime of current time
	/// @return	DateTime
	virtual DateTime now() const = 0;

	/// Set time to provided timezone
	/// @param[in]	time	Time to set to
	/// @param[in]	is_utc	True if 'time' is in UTC, false if local
	void			time_adjust(const DateTime time, const bool is_utc=true);

	/// Get timestamp
	/// @param[out]	header		Column header(s) of timestamp element
	/// @param[out]	timestamp	String to fill with timestamp element(s)
	/// @param[in]	delimiter	Delimiter to use
	/// @param[in]	format		How to format timestamp (0: no timestamp added, 1: only date added, 2: only time added, 3: both date and time added (two fields), 4: both date and time added (combined field) ), 
	void			get_timestamp(char* header, char* timestamp, const char delimiter, const uint8_t format=3);

	/// Set an alarm to go off at the specified time
	/// @param[in]	time	DateTime of time alarm should go off
	virtual void	set_alarm(DateTime time) = 0;

	/// Set an alarm for a duration
	/// @param[in]	duration	TimeSpan of duration before alarm goes off
	// virtual void	set_alarm(TimeSpan duration) = 0;
	void			set_alarm(const TimeSpan duration) { set_alarm(now()+duration); }

	/// Clear alarms
	virtual void	clear_alarms() = 0;

// Other functions that would be nice:
	// virtual TimeSpan	get_timer_remaining();
	// virtual void	pause_timer();
	// virtual void resume_timer();
	// virtual void restart_timer();

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() const override;
	virtual void	print_state() const override;

	/// Print an arbitrary DateTime object
	/// @param[in]	time	Time to print
	static void		print_DateTime(DateTime time);

	/// Print the current time
	/// @param[in]	verbose		True for multile lines, false for short display
	void			print_time(const bool verbose=false);

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================
	
	/// Get the pin the RTC interrupt is assumed to be connected to
	/// @return	Interrupt pin
	// byte			get_interrupt_pin();

	/// Get string of date
	/// @return	Date string
	const char*		get_datestring(); 
	
	/// Get string of date
	/// @param[out]	buf		Buffer to fill
	void			get_datestring(char* buf); 
	
	/// Get string of time
	/// @return	Time string
	const char*		get_timestring();
	
	/// Get string of time
	/// @param[out]	buf		Buffer to fill]	
	void			get_timestring(char* buf);
	
	/// Get string of weekday
	/// @return	Weekday string
	const char*		get_weekday() { return (const char*)daysOfTheWeek[ now().dayOfTheWeek() ]; }
	
	/// Get string of weekday
	/// @param[out]	buf		Buffer to fill
	void			get_weekday(char* buf);

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================
	
	void 			link_device_manager(LoomManager* LM) override;

	/// Get string of name associated with time zone enum
	/// @param[in]	t	TimeZone value to get string of
	/// @return C-string of time zone
	static char*	enum_timezone_string(const TimeZone t);

protected:

	/// Initialize RTC.
	/// Called by subclass constructors 
	void			init();


	// Because subclasses use use members that are not
	// polymorphic, the following _method are wrappers to 
	// the classes similarly named methods

	/// Begin auxiliary function that subclasses need to implement
	virtual void	_adjust(const DateTime time) = 0;

	/// Begin auxiliary function that subclasses need to implement.
	///	@return True if begin worked without issue, false otherwise
	virtual bool	_begin() = 0;

	/// Initialization auxiliary function that subclasses need to implement.
	/// @return True if RTC is initialized / did not lose power 
	virtual bool	_initialized() = 0;

	/// Read the RTC, update time and date strings
	void			read_rtc();

	/// Set the RTC time to compile time
	void			set_rtc_to_compile_time();

	/// Convert time between local and UTC.
	/// Uses current timezone setting
	/// @param[in]	to_utc	True to switch to UTC, false to go to local from UTC
	void			convert_local_to_utc(const bool to_utc=true);

	/// Check if current RTC time is valid (not necessarily correct)
	/// @return	True if valid
	bool			rtc_validity_check();

};


