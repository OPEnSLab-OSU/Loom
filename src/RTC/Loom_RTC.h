#pragma once

#include "Loom_Module.h"

#include <OPEnS_RTC.h>


///////////////////////////////////////////////////////////////////////////////


/// Abstract base of RTC modules
class LoomRTC : public LoomModule
{

public:

	/// Different time zones
	enum class TimeZone { 
		WAT, AT, ADT, AST, EDT, EST, CDT, CST, MDT, MST, PDT, PST, ALDT, 
		ALST, HST, SST, GMT, BST, CET, CEST, EET, EEST, BT, ZP4, ZP5, 
		ZP6, ZP7, AWST, AWDT, ACST, ACDT, AEST, AEDT 
	};

private: 
	
	const static char*	daysOfTheWeek[];		/// Array of strings the days of the week
	const static float	timezone_adjustment[];	/// Timezone hour adjustment associated with each TimeZone enum

protected:

	TimeZone	timezone;			/// The TimeZone to use

	bool		use_utc_time;		/// Whether or not use UTC time, else local time

	char		datestring[20];		/// Latest saved string of the Date (year/month/day)
	char		timestring[20];		/// Latest saved string of the time (hour:minute:second)

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomRTC(	
			const char*			module_name,
			LoomModule::Type	module_type,
			TimeZone			timezone,
			bool				use_utc_time
		);

	/// Destructor
	virtual ~LoomRTC() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

    void Run() {}
	virtual bool	dispatch(JsonObject) override {}
	virtual void 	package(JsonObject json) override;

	/// Get DateTime of current time
	/// \return	DateTime
	virtual DateTime now() = 0;

	/// Set time to provided timezone
	/// \param[in]	time	Time to set to
	/// \param[in]	is_utc	True if 'time' is in UTC, false if local
	void			time_adjust(DateTime time, bool is_utc=true);

	/// Get timestamp
	/// \param[out]	header		Column header(s) of timestamp element
	/// \param[out]	timestamp	String to fill with timestamp element(s)
	/// \param[in]	delimiter	Delimiter to use
	/// \param[in]	format		How to format timestamp (0: no timestamp added, 1: only date added, 2: only time added, 3: both date and time added (two fields), 4: both date and time added (combined field) ), 
	void			get_timestamp(char* header, char* timestamp, char delimiter, uint8_t format=3);

	/// Set an alarm to go off at the specified time
	/// \param[in]	time	DateTime of time alarm should go off
	virtual void	set_alarm(DateTime time) = 0;

	/// Set an alarm for a duration
	/// \param[in]	duration	TimeSpan of duration before alarm goes off
	// virtual void	set_alarm(TimeSpan duration) = 0;
	void			set_alarm(TimeSpan duration) { set_alarm(now()+duration); }


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

	virtual void 	print_config() override;
	virtual void 	print_state() override;

	/// Print an arbitrary DateTime object
	/// \param[in]	time	Time to print
	static void 	print_DateTime(DateTime time);

	/// Print the current time
	/// \param[in]	verbose		True for multile lines, false for short display
	void 			print_time(bool verbose=false);

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================
	
	/// Get the pin the RTC interrupt is assumed to be connected to
	/// \return	Interrupt pin
	// byte			get_interrupt_pin();

	/// Get string of date
	/// \return	Date string
	const char*		get_datestring(); 
	/// Get string of date
	/// \param[out]	buf		Buffer to fill
	void			get_datestring(char* buf); 
	/// Get string of time
	/// \return	Time string
	const char*		get_timestring();
	/// Get string of time
	/// \param[out]	buf		Buffer to fill]	
	void			get_timestring(char* buf);
	/// Get string of weekday
	/// \return	Weekday string
	const char*		get_weekday() { return (const char*)daysOfTheWeek[ now().dayOfTheWeek() ]; }
	/// Get string of weekday
	/// \param[out]	buf		Buffer to fill
	void			get_weekday(char* buf);

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================
	
	void 		link_device_manager(LoomManager* LM) override;

	/// Get string of name associated with time zone enum
	/// \return C-string of time zone
	static char*	enum_timezone_string(TimeZone t);




// Might be obsolete
	// Static because ISRs need to be static if they are class methods
	// Interrupt pin is also static, as that pin is referenced 
	// static void		RTC_Wake_ISR();


	

protected:

	/// Initialize RTC.
	/// Called by subclass constructors 
	void			init();

	// Because subclasses use use members that are not
	// polymorphic, they have to manager their own 

	/// Begin auxiliary function that subclasses need to implement
	virtual void	_adjust(DateTime time) = 0;

	/// Begin auxiliary function that subclasses need to implement
	virtual bool	_begin() = 0;

	/// Initialization auxiliary function that subclasses need to implement
	virtual bool	_initialized() = 0;

	/// Read the RTC, update time and date strings
	void			read_rtc();

	/// Set the RTC time to compile time
	void			set_rtc_to_compile_time();

	/// Convert time between local and UTC.
	/// Uses current timezone setting
	/// \param[in]	to_utc	True to switch to UTC, false to go to local from UTC
	void			convert_local_to_utc(bool to_utc=true);

	/// Check if current RTC time is valid (not necessarily correct)
	/// \return	True if valid
	bool			rtc_validity_check();

};


