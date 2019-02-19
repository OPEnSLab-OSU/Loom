
#include "loom_DS3231.h"



// --- CONSTRUCTOR ---
Loom_DS3231::Loom_DS3231(	char* 	module_name,
	 
							TimeZone 	timezone,
							bool 		use_utc_time,
							bool 		get_internet_time

	) : LoomRTC( module_name, timezone, use_utc_time, get_internet_time )
{
	rtc_inst = new RTC_DS3231();

	if (!rtc_inst->begin()) {
		print_module_label();
		Println("Couldn't find RTC");
		// while (1);	// remove while(1)
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
		if (rtc_inst->lostPower()) {
			Println("RTC 3231 lost power");
			set_rtc_to_compile_time();
		}

		// Make sure the RTC time is even valid, if not, set to compile time
		rtc_validity_check();

	} // of if (!internet_time_success)



	// Alarm setup
	// #if  is_rtc3231 == 1
	// 	clearRTCAlarms(); 	//clear any pending alarms

	// 	//Set SQW pin to OFF (in my case it was set by default to 1Hz)
	// 	//The output of the DS3231 INT pin is connected to this pin
	// 	rtc_inst->writeSqwPinMode(DS3231_OFF);

	// 	// Configure RTC Interrupt pin to be input
	// 	pinMode(RTC_pin, INPUT_PULLUP);
	// #endif	



	// Query Time and print
	print_time();
}

// --- DESTRUCTOR ---
Loom_DS3231::~Loom_DS3231() 
{
	delete rtc_inst;
}


// --- PUBLIC METHODS ---


void Loom_DS3231::print_config()
{
	LoomRTC::print_config();

	// will print out alarm info
}


DateTime Loom_DS3231::now()
{
	return rtc_inst->now();
}


void Loom_DS3231::time_adjust(DateTime time)
{
	rtc_inst->adjust(time);
}



