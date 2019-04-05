
#ifndef LOOM_DS3231_h
#define LOOM_DS3231_h


#include "Loom_RTC.h"


/// DS3231 RTC module
class Loom_DS3231 : public LoomRTC
{

protected:

	/// Underlying DS3231 manager instance
	RTC_DS3231*  rtc_inst;

public:

	// --- CONSTRUCTOR ---
	Loom_DS3231(	
			char*		module_name			=  "DS3231",  // DS3231_Module_Name,

			TimeZone	timezone			=  PST,  // DS3231_Timezone,
			bool		use_utc_time		=  false,  // DS3231_Use_Utc_Time,
			bool		get_internet_time	=  false,  // DS3231_Get_Internet_Time,
			
			byte		int_pin				= 6  // DS3231_Int_Pin 
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_DS3231();

	// Inherited (overriding) Methods
	void		print_config() override;
	bool		message_route(OSCMessage& msg, int address_offset) {};
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
