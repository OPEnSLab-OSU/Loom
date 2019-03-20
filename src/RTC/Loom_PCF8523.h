
#ifndef LOOM_PCF8523_h
#define LOOM_PCF8523_h

#include "Loom_RTC.h"



class Loom_PCF8523 : public LoomRTC
{
protected:

	/// Underlying PCF8523 manager instance
	// RTC_PCF8523*  rtc_inst;
	PCF8523*		rtc_inst;

public:

	// --- CONSTRUCTOR ---
	Loom_PCF8523(	
			char*		module_name			= "PCF8523",
	 
			TimeZone	timezone			= PST,
			bool		use_utc_time		= false,
			bool		get_internet_time	= false,

			byte		int_pin				= 6
		);


	// --- DESTRUCTOR ---
	virtual ~Loom_PCF8523();

	// Inherited (overriding) Methods
	void		print_config();
	bool		message_route(OSCMessage& msg, int address_offset) {}
	DateTime	now();

// proper alarms not really working yet
	void		set_alarm(DateTime time);
	void		set_alarm(TimeSpan duration);
	void		clear_alarms();

protected:

	bool		_begin();
	bool		_initialized();
	void		time_adjust(DateTime time);

};


#endif // of #ifndef LOOM_PCF8523_h
