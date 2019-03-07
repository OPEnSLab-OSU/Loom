
#ifndef LOOM_PCF8523_h
#define LOOM_PCF8523_h

#include "Loom_RTC.h"



class Loom_PCF8523 : public LoomRTC
{
protected:

	/// Underlying PCF8523 manager instance
	RTC_PCF8523*  rtc_inst;

public:

	// --- CONSTRUCTOR ---
	Loom_PCF8523(	char* 		module_name			= "PCF8523",
	 
					TimeZone 	timezone 			= PST,
					bool 		use_utc_time 		= false,
					bool 		get_internet_time 	= false,

					byte		int_pin				= 0
		   );

	// --- DESTRUCTOR ---
	virtual ~Loom_PCF8523();

	void print_config();
	bool message_route(OSCMessage& msg, int address_offset) {}
	
	DateTime now();


	// Alarm functions (not currently implemented for this RTC, 
	// need to combine 2 libraries to get this working)
	void set_alarm(DateTime time) {}
	void clear_alarms() {}

protected:

	void time_adjust(DateTime time);

};


#endif // of #ifndef LOOM_PCF8523_h
