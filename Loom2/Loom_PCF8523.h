
#ifndef LOOM_PCF8523_h
#define LOOM_PCF8523_h

#include "Loom_RTC.h"



class Loom_PCF8523 : public LoomRTC
{
protected:

	RTC_PCF8523*  rtc_inst;

public:

	// --- CONSTRUCTOR ---
	Loom_PCF8523(	char* 	module_name 			= "PCF8523",
	 
					TimeZone 	timezone 			= PST,
					bool 		use_utc_time 		= false,
					bool 		get_internet_time 	= false
		   );

	// --- DESTRUCTOR ---
	virtual ~Loom_PCF8523();


	void print_config();

	DateTime now();

	bool message_route(OSCMessage& msg, int address_offset) {}

protected:

	void time_adjust(DateTime time);

};


#endif // of #ifndef LOOM_PCF8523_h
