
#ifndef LOOM_DS3231_h
#define LOOM_DS3231_h


#include "loom_rtc_class.h"

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>



class Loom_DS3231 : public LoomRTC
{
protected:

	RTC_DS3231*  rtc_inst;

public:

	// --- PUBLIC MEMBERS ---


	// --- CONSTRUCTOR ---
	Loom_DS3231(	char* 	module_name 			= "DS3231",
	 
				TimeZone 	timezone 			= PST,
				bool 		use_utc_time 		= true,
				bool 		get_internet_time 	= false
		   );

	// --- DESTRUCTOR ---
	~Loom_DS3231();

	// --- PUBLIC METHODS ---

	void print_config();

	// void package(OSCBundle* bndl, char* id_prefix);


	DateTime now();

	bool message_route(OSCMessage* msg, int address_offset) {}


	// Alarm functions

protected:

	void time_adjust(DateTime time);

};


#endif // of #ifndef LOOM_3231_h