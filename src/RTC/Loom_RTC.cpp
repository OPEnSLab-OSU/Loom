
#include "Loom_RTC.h"
#include "Loom_Manager.h"
#include "../InternetPlats/Loom_InternetPlat.h"


#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>


///////////////////////////////////////////////////////////////////////////////
const char* LoomRTC::daysOfTheWeek[] = 
	{"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

///////////////////////////////////////////////////////////////////////////////
const float LoomRTC::timezone_adjustment[] =
	{
		1  /* WAT */, 2    /* AT  */, 3     /* ADT */, 4   /* AST */, 4   /* EDT */, 5  /* EST */, 5  /* CDT */,
		6  /* CST */, 6    /* MDT */, 7     /* MST */, 7   /* PDT */, 7   /* PST */, 8  /* ALDT*/, 9  /* ALST*/,
		10 /* HST */, 11   /* SST */, 0     /* GMT */, -1  /* BST */, -1  /* CET */, -2 /* CEST*/, -2 /* EET */,
		-3 /* EEST*/, -3   /* BT  */, -4    /* ZP4 */, -5  /* ZP5 */, -6  /* ZP6 */, -7 /* ZP7 */, -8 /* AWST*/,
		-9 /* AWDT*/, -9.5 /* ACST*/, -10.5 /* ACDT*/, -10 /* AEST*/, -11 /* AEDT*/
	};

///////////////////////////////////////////////////////////////////////////////
char* LoomRTC::enum_timezone_string(TimeZone t)
{
	switch(t) {
		case TimeZone::WAT  : return "WAT";
		case TimeZone::AT   : return "AT";
		case TimeZone::ADT  : return "ADT";
		case TimeZone::AST  : return "AST";
		case TimeZone::EDT  : return "EDT";
		case TimeZone::EST  : return "EST";
		case TimeZone::CDT  : return "CDT";
		case TimeZone::CST  : return "CST";
		case TimeZone::MDT  : return "MDT";
		case TimeZone::MST  : return "MST";
		case TimeZone::PDT  : return "PDT";
		case TimeZone::PST  : return "PST";
		case TimeZone::ALDT : return "ALDT";
		case TimeZone::ALST : return "ALST";
		case TimeZone::HST  : return "HST";
		case TimeZone::SST  : return "SST";
		case TimeZone::GMT  : return "GMT";
		case TimeZone::BST  : return "BST";
		case TimeZone::CET  : return "CET";
		case TimeZone::CEST : return "CEST";
		case TimeZone::EET  : return "EET";
		case TimeZone::EEST : return "EEST";
		case TimeZone::BT   : return "BT";
		case TimeZone::ZP4  : return "ZP4";
		case TimeZone::ZP5  : return "ZP5";
		case TimeZone::ZP6  : return "ZP6";
		case TimeZone::ZP7  : return "ZP7";
		case TimeZone::AWST : return "AWST";
		case TimeZone::AWDT : return "AWDT";
		case TimeZone::ACST : return "ACST";
		case TimeZone::ACDT : return "ACDT";
		case TimeZone::AEST : return "AEST";
		case TimeZone::AEDT : return "AEDT";
	}
}

///////////////////////////////////////////////////////////////////////////////
LoomRTC::LoomRTC(	
		const char*			module_name,
		LoomModule::Type	module_type,
		TimeZone			timezone,
		bool				use_utc_time,
		bool				get_internet_time
	) 
	: LoomModule( module_name, module_type )
	, timezone(timezone)
	, use_utc_time(use_utc_time)
	, get_internet_time(get_internet_time)
{}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::print_config()
{
	LoomModule::print_config();
	LPrintln("\tUse UTC Time      : ", use_utc_time);
	LPrintln("\tGet Internet Time : ", get_internet_time);
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::print_state()
{
	LoomModule::print_state();
	print_time();
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::package(JsonObject json)
{
	read_rtc();
	package_json_timestamp(json, datestring, timestring);
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::print_time(bool verbose)
{
	read_rtc();
	print_module_label();

	if (verbose) {
		LPrintln("Time : ");
		LPrintln("\tDate: ", get_datestring());
		LPrintln("\tTime: ", get_timestring());
		LPrintln("\tDay : ", get_weekday());
		LPrintln();
	} else {
		LPrint("Time: ");
		print_DateTime( now() );
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::print_DateTime(DateTime time) 
{
	LPrint(time.year());   LPrint('/');
	LPrint(time.month());  LPrint('/');
	LPrint(time.day());    LPrint(' ');
	LPrint(time.hour());   LPrint(':');
	LPrint(time.minute()); LPrint(':');
	LPrintln(time.second()); 
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::read_rtc()
{
	get_datestring();
	get_timestring();
}

///////////////////////////////////////////////////////////////////////////////
const char* LoomRTC::get_datestring()
{
	DateTime time = now();
	sprintf(datestring, "%d/%d/%d", time.year(), time.month(), time.day() );
	return datestring;
} 

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::get_datestring(char* buf)
{
	DateTime time = now();
	sprintf(buf, "%d/%d/%d", time.year(), time.month(), time.day() );
	strcpy(datestring, buf);
} 

///////////////////////////////////////////////////////////////////////////////
const char* LoomRTC::get_timestring()
{
	DateTime time = now();
	sprintf(timestring, "%d:%d:%d", time.hour(), time.minute(), time.second() );
	return timestring;
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::get_timestring(char* buf)
{
	DateTime time = now();
	sprintf(buf, "%d:%d:%d", time.hour(), time.minute(), time.second() );
	strcpy(timestring, buf);
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::get_weekday(char* buf)
{
	strcpy( buf, daysOfTheWeek[ now().dayOfTheWeek() ] );
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::get_timestamp(char* header, char* timestamp, char delimiter, uint8_t format)
{
	switch (format) {
		case 1 :
			sprintf(header, "Date%c", delimiter); 
			sprintf(timestamp, "%s%c ", get_timestring() ); 
			break;
		case 2 :
			sprintf(header, "Date%c", delimiter); 
			sprintf(timestamp, "%s%c ", get_datestring() ); 
			break;
		case 3 :
			sprintf(header, "Date%c Time%c ", delimiter, delimiter); 
			sprintf(timestamp, "%s%c %s%c ", get_datestring(), delimiter, get_timestring(), delimiter ); 
			break;
		case 4 :
			sprintf(header, "Date_Time", delimiter); 
			sprintf(timestamp, "%s %s%c ", get_datestring(), get_timestring(), delimiter ); 
			break;
		default : 
			strcpy(header, ""); 
			strcpy(timestamp, ""); 
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::init()
{
	if (!_begin()) {
		print_module_label();
		LPrintln("RTC not found");
		return;
	}

	print_module_label();
	LPrintln("Current Time (before possible reset)");
	print_time();

	// bool internet_time_success = false;

	// // Try to set the time from internet
	// if (get_internet_time) { 
	// 	internet_time_success = set_rtc_from_internet_time();
	// }

	// // If unable to set time via internet, default to normal behavior
	// if (!internet_time_success) {

	// 	// The following section checks if RTC is running, else sets 
	// 	// the time to the time that the sketch was compiled
	// 	if (!_initialized()) {
	// 		print_module_label();
	// 		LPrintln("RTC was not initialized");
	// 		set_rtc_to_compile_time();
	// 	}

	// 	// Make sure the RTC time is even valid, if not, set to compile time
	// 	rtc_validity_check();

	// } // of if (!internet_time_success)



	// The following section checks if RTC is running, else sets 
	// the time to the time that the sketch was compiled
	if (!_initialized()) {
		print_module_label();
		LPrintln("RTC was not initialized");
		set_rtc_to_compile_time();
	}

	// Make sure the RTC time is even valid, if not, set to compile time
	rtc_validity_check();


	// Query Time and print
	print_time();
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::set_rtc_to_compile_time()
{
	// This sets to local time zone
	time_adjust( DateTime(F(__DATE__), F(__TIME__)) );
	
	print_module_label();
	LPrintln("Time set to compile time:");
	print_time();

	// Adjust to UTC time if enabled
	if (use_utc_time) convert_local_to_utc();
}

///////////////////////////////////////////////////////////////////////////////
bool LoomRTC::set_rtc_from_internet_time()
{
	uint32_t unixTime = 0;
	bool internet_time_success = false;

	// Check device manager for an internet platform
	// Query platform for time
	if (device_manager != nullptr) {
		if (device_manager->InternetPlat(0).is_connected() ) {
			unixTime = device_manager->InternetPlat(0).get_time();
			print_module_label();
			LPrintln("Unix Time: ", unixTime);
		}
	}


	if (unixTime != 0) {
		// Set to UTC time
		time_adjust(DateTime(unixTime));

		print_module_label();
		LPrintln("Time set to:");
		print_time();

		// If not using UTC Time convert to local
		if (!use_utc_time) convert_local_to_utc(false);

		internet_time_success = true;
	} 

	print_module_label();
	LPrintln("Set time from internet ", (internet_time_success) ? "successful" : "failed");

	return internet_time_success;
} 

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::convert_local_to_utc(bool to_utc)
{
	float adj = ( (to_utc) ? 1. : -1. ) * timezone_adjustment[(int)timezone];
	int min;

	if ( (adj-(int)adj) == 0 ) { 
		min = 0;
	} else if ( (adj-(int)adj) > 0 ) { 
		min = 30;
	} else { 
		min = -30; 
	}

	DateTime utc = now() + TimeSpan(0, (int)adj, min, 0);
	time_adjust(utc);

	print_module_label();
	LPrintln("Time adjusted to ", (to_utc) ? "UTC" : "Local" , ": ");
	print_time();
	LPrintln();
}

///////////////////////////////////////////////////////////////////////////////
bool LoomRTC::rtc_validity_check()
{
	DateTime time_check = now();
	int y = time_check.year();
	int m = time_check.month();
	int d = time_check.day();

	// A basic validity check of date
	if ( (y < 2018) || (y > 2050) || (m < 1) || (m > 12) || (d < 1) || (d > 31) ) {
		print_module_label();
		LPrintln("RTC Time is invalid");
		set_rtc_to_compile_time();
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
