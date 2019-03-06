
#ifndef LOOM_DS3231_h
#define LOOM_DS3231_h


#include "Loom_RTC.h"

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>



class Loom_DS3231 : public LoomRTC
{
protected:

	RTC_DS3231*  rtc_inst;

public:

	// --- CONSTRUCTOR ---
	Loom_DS3231(	char* 	module_name 			= "DS3231",
	 
					TimeZone 	timezone 			= PST,
					bool 		use_utc_time 		= false,
					bool 		get_internet_time 	= false,
					
					byte		int_pin				= 6

		   );

	// --- DESTRUCTOR ---
	virtual ~Loom_DS3231();

	void print_config();
	bool message_route(OSCMessage& msg, int address_offset) {}

	DateTime now();


	// Alarm functions
	void set_alarm(DateTime time);
	void clear_alarms();


protected:

	void time_adjust(DateTime time);

};


#endif // of #ifndef LOOM_DS3231_h
