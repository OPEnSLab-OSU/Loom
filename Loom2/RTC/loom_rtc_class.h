
#ifndef LOOM_RTC_h
#define LOOM_RTC_h

// ABSTRACT CLASS - ACTUAL RTC CLOCKS IMPLEMENTED AS SUBCLASSES

#include "loom_abstract_module_class.h"

#include <RTClibExtended.h>
// NOTE: Must include the following line in the RTClibExtended.h file to use with M0:
// #define _BV(bit) (1 << (bit))




enum TimeZone { 
	WAT, AT, ADT, AST, EDT, EST, CDT, CST, MDT, MST, PDT, PST, ALDT, 
	ALST, HST, SST, GMT, BST, CET, CEST, EET, EEST, BT, ZP4, ZP5, 
	ZP6, ZP7, AWST, AWDT, ACST, ACDT, AEST, AEDT 
};



// const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// const float timezone_adjustment[34] =
// {
// 	1  /* WAT */, 2    /* AT  */, 3     /* ADT */, 4   /* AST */, 4   /* EDT */, 5  /* EST */, 5  /* CDT */,
// 	6  /* CST */, 6    /* MDT */, 7     /* MST */, 7   /* PDT */, 8   /* PST */, 8  /* ALDT*/, 9  /* ALST*/,
// 	10 /* HST */, 11    SST , 0     /* GMT */, -1  /* BST */, -1  /* CET */, -2 /* CEST*/, -2 /* EET */,
// 	-3 /* EEST*/, -3   /* BT  */, -4    /* ZP4 */, -5  /* ZP5 */, -6  /* ZP6 */, -7 /* ZP7 */, -8 /* AWST*/,
// 	-9 /* AWDT*/, -9.5 /* ACST*/, -10.5 /* ACDT*/, -10 /* AEST*/, -11 /* AEDT*/
// };


class LoomRTC : public LoomModule
{
private: 
	// static const char* const daysOfTheWeek[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	const static char* daysOfTheWeek[];
	const static float timezone_adjustment[];

protected:
	// --- PROTECTED MEMBERS ---

	TimeZone 	timezone;
	bool 		use_utc_time;
	bool 		get_internet_time;

	char	 	datestring[20];
	char 		timestring[20];

	// maybe store basic seconds unit?

public:

	// --- PUBLIC MEMBERS ---


	// --- CONSTRUCTOR ---
	LoomRTC(	char* 	module_name,
	 
				TimeZone 	timezone 			= PST,
				bool 		use_utc_time 		= true,
				bool 		get_internet_time 	= false
		   );

	// --- DESTRUCTOR ---
	~LoomRTC();


	static char* enum_timezone_string(TimeZone t);

	// --- PUBLIC METHODS ---

	virtual void print_config();

	void print_time();

	virtual void measure();

	virtual void package(OSCBundle* bndl, char* id_prefix);


	// virtual bool message_route(OSCMessage* msg, int address_offset) = 0;


	char* get_datestring(); 
	void  get_datestring(char* buf); 
	char* get_timestring();	
	void  get_timestring(char* buf);
	char* get_weekday();
	void  get_weekday(char* buf);


	virtual DateTime now() = 0;


protected:

// --- PRIVATE MEMBERS ---

// --- PRIVATE METHODS --- 

	// Similar to measure_rtc  -- might not be needed
	void read_rtc();

	void set_rtc_to_compile_time();


// needs to reference and internet connectivity class to get unix time
	bool set_rtc_from_internet_time(); 

	void convert_local_to_utc(bool to_utc);

	void convert_local_to_utc();

	bool rtc_validity_check();

	static void print_DateTime(DateTime time);

	virtual void time_adjust(DateTime time) = 0;


};


#endif // of #ifndef LOOM_RTC_h