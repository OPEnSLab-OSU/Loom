
#ifndef LOOM_RTC_h
#define LOOM_RTC_h


#include "Loom_Module.h"

// #include <RTClibExtended.h>
// // NOTE: Must include the following line in the RTClibExtended.h file to use with M0:
// // #define _BV(bit) (1 << (bit))

#include <OPEnS_RTC.h>


// byte RTC_Int_Pin = 0;


/// Different time zones
enum class TimeZone { 
	WAT, AT, ADT, AST, EDT, EST, CDT, CST, MDT, MST, PDT, PST, ALDT, 
	ALST, HST, SST, GMT, BST, CET, CEST, EET, EEST, BT, ZP4, ZP5, 
	ZP6, ZP7, AWST, AWDT, ACST, ACDT, AEST, AEDT 
};



// const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// const float timezone_adjustment[34] =
// {
// 	1  /* WAT */, 2    /* AT  */, 3     /* ADT */, 4   /* AST */, 4   /* EDT */, 5  /* EST */, 5  /* CDT */,
// 	6  /* CST */, 6    /* MDT */, 7     /* MST */, 7   /* PDT */, 8    PST , 8  /* ALDT*/, 9  /* ALST*/,
// 	10 /* HST */, 11   /* SST */, 0     /* GMT */, -1  /* BST */, -1  /* CET */, -2 /* CEST*/, -2 /* EET */,
// 	-3 /* EEST*/, -3   /* BT  */, -4    /* ZP4 */, -5  /* ZP5 */, -6  /* ZP6 */, -7 /* ZP7 */, -8 /* AWST*/,
// 	-9 /* AWDT*/, -9.5 /* ACST*/, -10.5 /* ACDT*/, -10 /* AEST*/, -11 /* AEDT*/
// };



// void RTC_Wake_ISR();
// {
// 	// Detach interrupt to prevent duplicate triggering
// 	// detachInterrupt(digitalPinToInterrupt(int_pin));
// 	detachInterrupt(digitalPinToInterrupt(RTC_Int_Pin));

// 	// Nothing else to do because interrupt woke device
// }


/// Abstract base of RTC modules
class LoomRTC : public LoomModule
{

private: 

	// static const char* const daysOfTheWeek[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	
	/// Array of strings the days of the week
	const static char*	daysOfTheWeek[];
	/// Timezone hour adjustment associated with each TimeZone enum
	const static float	timezone_adjustment[];

protected:

	/// The TimeZone to use
	TimeZone	timezone;

	/// Whether or not use UTC time, else local time
	bool		use_utc_time;
	/// Whether or not to try to get the time over an enabled internet platform
	bool		get_internet_time;

	/// Latest saved string of the Date (year/month/day)
	char		datestring[20];
	/// Latest saved string of the time (hour:minute:second)
	char		timestring[20];

	// Which pin the RTC interrupt is connected to (static so RTC ISR can be static)
	static byte	int_pin; 

public:

	// --- CONSTRUCTOR ---
	LoomRTC(	
			const char*		module_name			= "RTC",
 
			TimeZone		timezone			= TimeZone::PST,
			bool			use_utc_time		= true,
			bool			get_internet_time	= false,

			byte			interrupt_pin		= 6
		);

	// --- DESTRUCTOR ---
	virtual ~LoomRTC();

	static char*	enum_timezone_string(TimeZone t);

	// Inherited Methods
	virtual void 	print_config() override;
	virtual void 	print_state() override;
	virtual void 	measure() {};
	virtual void 	package(OSCBundle& bndl, char* suffix="") override;
	virtual bool 	message_route(OSCMessage& msg, int address_offset) = 0;
	virtual void 	package(JsonObject json) override;


	/// Get DateTime of current time
	/// \return	DateTime
	virtual DateTime now() = 0;

	/// LPrint an arbitrary DateTime object
	/// \param[in]	time	Time to print
	static void 	print_DateTime(DateTime time);

	/// LPrint the current time
	/// \param[in]	verbose		True for multile lines, false for short display
	void 			print_time(bool verbose=false);


	/// Get string of date
	/// \return	Date string
	char*			get_datestring(); 
	/// Get string of date
	/// \param[out]	buf		Buffer to fill
	void			get_datestring(char* buf); 
	/// Get string of time
	/// \return	Time string
	char*			get_timestring();
	/// Get string of time
	/// \param[out]	buf		Buffer to fill]	
	void			get_timestring(char* buf);
	/// Get string of weekday
	/// \return	Weekday string
	char*			get_weekday();
	/// Get string of weekday
	/// \param[out]	buf		Buffer to fill
	void			get_weekday(char* buf);


	/// Get timestamp
	/// \param[out]	header		Column header(s) of timestamp element
	/// \param[out]	timestamp	String to fill with timestamp element(s)
	/// \param[in]	delimiter	Delimiter to use
	/// \param[in]	format		How to format timestamp (0: no timestamp added, 1: only date added, 2: only time added, 3: both date and time added (two fields), 4: both date and time added (combined field) ), 
	void			get_timestamp(char* header, char* timestamp, char delimiter, uint8_t format=3);



	virtual void	set_alarm(DateTime time) = 0;
	virtual void	set_alarm(TimeSpan duration) = 0;

	virtual void	clear_alarms() {}

// Other functions that would be nice:
	// virtual TimeSpan	get_timer_remaining();
	// virtual void	pause_timer();
	// virtual void resume_timer();
	// virtual void restart_timer();



	/// Get the pin the RTC interrupt is assumed to be connected to
	/// \return	Interrupt pin
	byte			get_interrupt_pin();

	// Static because ISRs need to be static if they are class methods
	// Interrupt pin is also static, as that pin is referenced 
	static void		RTC_Wake_ISR();


protected:

	/// Initialize RTC.
	/// Called by subclass constructors 
	void			init();
	// Because subclasses use use members that are not
	// polymorphic, they have to manager their own 
	virtual bool	_begin() = 0;
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

	/// Set time to provided timezone
	/// \param[in]	time	Time to set to
	virtual void	time_adjust(DateTime time) = 0;



// needs to reference and internet connectivity class to get unix time

	/// Try to set RTC to time from internet 
	/// \return	True if success
	bool			set_rtc_from_internet_time(); 

};



#endif // of #ifndef LOOM_RTC_h
