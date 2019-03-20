
#ifndef LOOM_DS3231_h
#define LOOM_DS3231_h


#include "Loom_RTC.h"



class Loom_DS3231 : public LoomRTC
{
protected:

	/// Underlying DS3231 manager instance
	RTC_DS3231*  rtc_inst;

public:

	// --- CONSTRUCTOR ---
	Loom_DS3231(	
			char*		module_name			= DS3231_Module_Name, // "DS3231",

			TimeZone	timezone			= DS3231_Timezone, // PST,
			bool		use_utc_time		= DS3231_Use_Utc_Time, // false,
			bool		get_internet_time	= DS3231_Get_Internet_Time, // false,
			
			byte		int_pin				= DS3231_Int_Pin // 6
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_DS3231();

	// Inherited (overriding) Methods
	void		print_config();
	bool		message_route(OSCMessage& msg, int address_offset) {}
	DateTime	now();
	void		set_alarm(DateTime time);
	void		set_alarm(TimeSpan duration);
	void		clear_alarms();


protected:

	bool		_begin();
	bool		_initialized();
	void		time_adjust(DateTime time);

};


#endif // of #ifndef LOOM_DS3231_h
