///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_RTC.cpp
/// @brief		File for LoomRTC implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "RTC.h"
#include "Manager.h"
#include "../InternetPlats/InternetPlat.h"
#include "../Interrupt_Manager.h"


#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>


///////////////////////////////////////////////////////////////////////////////
const char* LoomRTC::daysOfTheWeek[] = 
	{"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

///////////////////////////////////////////////////////////////////////////////
const float LoomRTC::timezone_adjustment[] =
	{
		1  /* WAT */, 2    /* AT  */, 3     /* ADT */, 4   /* AST */, 4   /* EDT */, 5  /* EST */, 5  /* CDT */,
		6  /* CST */, 6    /* MDT */, 7     /* MST */, 7   /* PDT */, 8   /* PST */, 8  /* ALDT*/, 9  /* ALST*/,
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
		LoomManager* 			manager,
		const char*							module_name,
		const LoomModule::Type	module_type,
		const TimeZone					timezone,
		const bool							use_utc_time
	) 
	: LoomModule(manager, module_name, module_type )
	, timezone(timezone)
	, use_utc_time(use_utc_time)
{}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::print_config() const
{
	LoomModule::print_config();
	LPrintln("\tUse UTC Time      : ", use_utc_time);
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::print_state() const
{
	LoomModule::print_state();
	// print_time();
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::package(JsonObject json)
{
	read_rtc();
	package_json_timestamp(json, datestring, timestring);
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::print_time(const bool verbose)
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
void LoomRTC::print_DateTime(const DateTime time)
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
void LoomRTC::get_timestamp(char* header, char* timestamp, const char delimiter, const uint8_t format)
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
	_adjust( DateTime(F(__DATE__), F(__TIME__)) );
	
	print_module_label();
	LPrintln("Time set to compile time:");
	print_time();

	// Adjust to UTC time if enabled
	if (use_utc_time) convert_local_to_utc();
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::convert_local_to_utc(const bool to_utc)
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

	DateTime utc_time = now() + TimeSpan(0, (int)adj, min, 0);

	print_module_label();
	LPrintln("Adjusting time to ", (to_utc) ? "UTC" : "Local");

	_adjust(utc_time);
}

///////////////////////////////////////////////////////////////////////////////
bool LoomRTC::rtc_validity_check()
{
	DateTime time_check = now();
	int y = time_check.year();

	// A basic validity check of date
	if ( (y < 2018) || (y > 2050)) {
		print_module_label();
		LPrint("RTC Time is invalid: ", y, '\n');
		set_rtc_to_compile_time();
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::link_device_manager(LoomManager* LM)
{
	LoomModule::link_device_manager(LM);

	if (LM) {
		// Set manager's interrupt manager 
		LM->rtc_module = this;

		auto interrupt_manager = LM->get_interrupt_manager();
		if (interrupt_manager) {
			interrupt_manager->set_RTC_module( this );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::time_adjust(const DateTime time, const bool is_utc)
{
	_adjust(time);

	// Check if source time is not in desired mode
	if (use_utc_time != is_utc) {
		convert_local_to_utc(use_utc_time);
	}

	print_module_label();
	LPrint("Adjusted time to: "); 
	print_DateTime(now());	
}

///////////////////////////////////////////////////////////////////////////////

