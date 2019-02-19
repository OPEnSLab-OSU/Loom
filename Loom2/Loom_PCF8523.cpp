
#include "Loom_PCF8523.h"
#include "Loom_Macros.h"






// --- CONSTRUCTOR ---
Loom_PCF8523::Loom_PCF8523(	char* 	module_name,
	 
						TimeZone 	timezone,
						bool 		use_utc_time,
						bool 		get_internet_time

	) : LoomRTC( module_name, timezone, use_utc_time, get_internet_time )
{
	rtc_inst = new RTC_PCF8523();

	if (!rtc_inst->begin()) {
		print_module_label();
		Println("Couldn't find RTC");
	}

	Println("\nCurrent Time (before possible resetting)");
	print_time();


	bool internet_time_success = false;

	// Try to set the time from internet
	if (get_internet_time) { 
		internet_time_success = set_rtc_from_internet_time();
	}

	// If unable to set time via internet, default to normal behavior
	if (!internet_time_success) {

		// The following section checks if RTC is running, else sets 
		// the time to the time that the sketch was compiled
		if (!rtc_inst->initialized()) {
			print_module_label();
			Println("RTC 8523 was not initialized");
			set_rtc_to_compile_time();
		}

		// Make sure the RTC time is even valid, if not, set to compile time
		rtc_validity_check();

	} // of if (!internet_time_success)


	// Query Time and print
	print_time();
}


// --- DESTRUCTOR ---
Loom_PCF8523::~Loom_PCF8523() 
{
	delete rtc_inst;
}	


// --- PUBLIC METHODS ---


void Loom_PCF8523::print_config()
{
	LoomRTC::print_config();
}


DateTime Loom_PCF8523::now()
{
	return rtc_inst->now();
}


void Loom_PCF8523::time_adjust(DateTime time)
{
	rtc_inst->adjust(time);
}



