
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
			char*		module_name			= PCF8523_Module_Name, // "PCF8523",
	 
			TimeZone	timezone			= PCF8523_Timezone, // PST,
			bool		use_utc_time		= PCF8523_Use_Utc_Time, // false,
			bool		get_internet_time	= PCF8523_Get_Internet_Time, // false,

			byte		int_pin				= PCF8523_Int_Pin // 6
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
