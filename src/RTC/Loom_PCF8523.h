
#ifndef LOOM_PCF8523_h
#define LOOM_PCF8523_h

#include "Loom_RTC.h"


/// PCF8523 RTC module
class Loom_PCF8523 : public LoomRTC
{
	
protected:

	/// Underlying PCF8523 manager instance
	PCF8523*		rtc_inst;
	// RTC_PCF8523*  rtc_inst;

public:

	// --- CONSTRUCTOR ---
	Loom_PCF8523(	
			char*		module_name			=  "PCF8523",   // PCF8523_Module_Name,
	 
			TimeZone	timezone			=  PST,   // PCF8523_Timezone,
			bool		use_utc_time		=  false,   // PCF8523_Use_Utc_Time,
			bool		get_internet_time	=  false,   // PCF8523_Get_Internet_Time,

			byte		int_pin				= 6   // PCF8523_Int_Pin 
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
