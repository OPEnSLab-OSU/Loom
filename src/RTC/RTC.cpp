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
		// All of the number represents the difference between UTC
		1  /* WAT */,

		2  /* AT  */,

		3  /* ADT */,

		4  /* AST */,

		4  /* EDT */,

		5  /* EST */,

		5  /* CDT */,

		6  /* CST */,

		6  /* MDT */,

		7  /* MST */,

		7  /* PDT */,

		8  /* PST */,

		8  /* AKDT */,

		9  /* AKST */,

		9 /* HST */,

		11 /* SST */,

		0  /* GMT */,

		-1 /* BST */,

		-1 /* CET */,

		-2 /* EET */,

		-3 /* EEST */,

		-3 /* BRT  */,

		-4 /* ZP4 */,

		-5 /* ZP5 */,

		-6 /* ZP6 */,

		-7 /* ZP7 */,

		-8 /* AWST*/,

		-9.5 /* ACST*/,

		-10 /* AEST*/
	};

///////////////////////////////////////////////////////////////////////////////
char* LoomRTC::enum_timezone_string(TimeZone t)
{
  LMark;
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
		case TimeZone::AKDT : return "AKDT";
		case TimeZone::AKST : return "AKST";
		case TimeZone::HST  : return "HST";
		case TimeZone::SST  : return "SST";
		case TimeZone::GMT  : return "GMT";
		case TimeZone::BST  : return "BST";
		case TimeZone::CET  : return "CET";
		case TimeZone::EET  : return "EET";
		case TimeZone::EEST : return "EEST";
		case TimeZone::BRT   : return "BRT";
		case TimeZone::ZP4  : return "ZP4";
		case TimeZone::ZP5  : return "ZP5";
		case TimeZone::ZP6  : return "ZP6";
		case TimeZone::ZP7  : return "ZP7";
		case TimeZone::AWST : return "AWST";
		case TimeZone::ACST : return "ACST";
		case TimeZone::AEST : return "AEST";
	}
}

///////////////////////////////////////////////////////////////////////////////
LoomRTC::LoomRTC(
		LoomManager* 			manager,
		const char*							module_name,
		const LoomModule::Type	module_type,
		TimeZone					timezone,
		const bool							use_local_time,
		const bool				custom_time
	)
	: LoomModule(manager, module_name, module_type )
	, timezone(timezone)
	, use_local_time(use_local_time)
	, converted(false)
	, local_time(0)
	, custom_time(custom_time)

{}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::print_config() const
{
  LMark;
	LoomModule::print_config();
  LMark;
	LPrintln("\tUse UTC Time      : ", use_local_time);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::print_state() const
{
  LMark;
	LoomModule::print_state();
  LMark;
	// print_time();
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::package(JsonObject json)
{
  LMark;
	read_rtc();
  LMark;
	package_json_timestamp(json, datestring, timestring);
  LMark;
	if (use_local_time){
   	LMark;
		local_rtc();
   	LMark;
		JsonObject data = get_module_data_object(json, "LocalTime");
   	LMark;
		sprintf(local_datestring, "%d/%d/%d", local_time.year(), local_time.month(), local_time.day());
   	LMark;
		sprintf(local_timestring, "%d:%d:%d", local_time.hour(), local_time.minute(), local_time.second());
   	LMark;
		data["Date"] = local_datestring;
   	LMark;
		data["Time"] = local_timestring;
   	LMark;
		data["TimeZone"] = enum_timezone_string(timezone);
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::print_time(const bool verbose)
{
  LMark;
	read_rtc();
  LMark;
	print_module_label();
  LMark;

	if (verbose) {
   	LMark;
		LPrintln("Time : ");
   	LMark;
		LPrintln("\tDate: ", get_datestring());
   	LMark;
		LPrintln("\tTime: ", get_timestring());
   	LMark;
		LPrintln("\tDay : ", get_weekday());
   	LMark;
		LPrintln();
  	LMark;
	} else {
		LPrint("Time: ");
   	LMark;
		print_DateTime( now() );
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::print_DateTime(const DateTime time)
{
  LMark;
	LPrint(time.year());   LPrint('/');
  LMark;
	LPrint(time.month());  LPrint('/');
  LMark;
	LPrint(time.day());    LPrint(' ');
  LMark;
	LPrint(time.hour());   LPrint(':');
  LMark;
	LPrint(time.minute()); LPrint(':');
  LMark;
	LPrintln(time.second());
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::read_rtc()
{
  LMark;
	get_datestring();
  LMark;
	get_timestring();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::local_rtc(){
  LMark;
	local_time = now();
  LMark;
	float adj = -1. * timezone_adjustment[(int)timezone];
  LMark;
	int min;
  LMark;

	if ( (adj-(int)adj) == 0 ) {
   	LMark;
		min = 0;
  	LMark;
	} else if ( (adj-(int)adj) > 0 ) {
		min = 30;
  	LMark;
	} else {
		min = -30;
  	LMark;
	}

	local_time = local_time + TimeSpan(0, (int)adj, min, 0);
  LMark;
	local_time = convert_daylight_to_standard(local_time);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
const char* LoomRTC::get_datestring()
{
  LMark;
	DateTime time = now();
  LMark;
	sprintf(datestring, "%d/%d/%d", time.year(), time.month(), time.day() );
  LMark;
	return datestring;
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::get_datestring(char* buf)
{
  LMark;
	DateTime time = now();
  LMark;
	sprintf(buf, "%d/%d/%d", time.year(), time.month(), time.day() );
  LMark;
	strcpy(datestring, buf);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
const char* LoomRTC::get_timestring()
{
  LMark;
	DateTime time = now();
  LMark;
	sprintf(timestring, "%d:%d:%d", time.hour(), time.minute(), time.second() );
  LMark;
	return timestring;
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::get_timestring(char* buf)
{
  LMark;
	DateTime time = now();
  LMark;
	sprintf(buf, "%d:%d:%d", time.hour(), time.minute(), time.second() );
  LMark;
	strcpy(timestring, buf);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::get_weekday(char* buf)
{
  LMark;
	strcpy( buf, daysOfTheWeek[ now().dayOfTheWeek() ] );
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::get_timestamp(char* header, char* timestamp, const char delimiter, const uint8_t format)
{
  LMark;
	switch (format) {
		case 1 :
    	LMark;
			sprintf(header, "Date%c", delimiter);
    	LMark;
			sprintf(timestamp, "%s%c ", get_timestring() );
    	LMark;
			break;
		case 2 :
    	LMark;
			sprintf(header, "Date%c", delimiter);
    	LMark;
			sprintf(timestamp, "%s%c ", get_datestring() );
    	LMark;
			break;
		case 3 :
    	LMark;
			sprintf(header, "Date%c Time%c ", delimiter, delimiter);
    	LMark;
			sprintf(timestamp, "%s%c %s%c ", get_datestring(), delimiter, get_timestring(), delimiter );
    	LMark;
			break;
		case 4 :
    	LMark;
			sprintf(header, "Date_Time", delimiter);
    	LMark;
			sprintf(timestamp, "%s %s%c ", get_datestring(), get_timestring(), delimiter );
    	LMark;
			break;
		default :
    	LMark;
			strcpy(header, "");
    	LMark;
			strcpy(timestamp, "");
  		LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::init()
{
  LMark;
	if (!_begin()) {
   	LMark;
		print_module_label();
   	LMark;
		LPrintln("RTC not found");
   	LMark;
		return;
	}

	print_module_label();
  LMark;
	LPrintln("Current Time (before possible reset)");
  LMark;
	print_time();
  LMark;

	// The following section checks if RTC is running, else sets
	// the time to the time that the sketch was compiled
	if (!_initialized()) {
   	LMark;
		print_module_label();
   	LMark;
		LPrintln("RTC was not initialized");
   	LMark;
		if(Serial && custom_time){
    	LMark;
			set_rtc_to_computer_time();
   		LMark;
		}
		else{
    	LMark;
			set_rtc_to_compile_time();
   		LMark;
		}
	}

	// Make sure the RTC time is even valid, if not, set to compile time
	rtc_validity_check();
  LMark;

	// Query Time and print
	print_time();
 	LMark;
}
///////////////////////////////////////////////////////////////////////////////
void LoomRTC::set_rtc_to_computer_time()
{
  LMark;

	// initialized variable for user input
	String computer_year = "";
  LMark;
	String computer_month = "";
  LMark;
	String computer_day = "";
  LMark;
	String computer_hour = "";
  LMark;
	String computer_min = "";
  LMark;
	String computer_sec = "";
  LMark;

	// Let the user know that they should enter local time
	print_module_label();
  LMark;
	LPrintln("Please use your local time, not UTC!");
  LMark;

	// Entering the year
	print_module_label();
  LMark;
	LPrintln("Enter the Year");
  LMark;
	while(computer_year == ""){
   	LMark;
		computer_year = Serial.readStringUntil('\n');
  	LMark;
	}
	print_module_label();
  LMark;
	LPrintln("Year Entered: ", computer_year);
  LMark;

	// Entering the month
	print_module_label();
  LMark;
	LPrintln("Enter the Month");
  LMark;
	while(computer_month == ""){
   	LMark;
		computer_month = Serial.readStringUntil('\n');
  	LMark;
	}
	print_module_label();
  LMark;
	LPrintln("Month Entered: ", computer_month);
  LMark;

	// Entering the day
	print_module_label();
  LMark;
	LPrintln("Enter the Day");
  LMark;
	while(computer_day  == ""){
   	LMark;
		computer_day = Serial.readStringUntil('\n');
  	LMark;
	}
	print_module_label();
  LMark;
	LPrintln("Day Entered: ", computer_day);
  LMark;

	// Entering the hour
	print_module_label();
  LMark;
	LPrintln("Enter the Hour (0 ~ 23)");
  LMark;
	while(computer_hour == ""){
   	LMark;
		computer_hour = Serial.readStringUntil('\n');
  	LMark;
	}
	print_module_label();
  LMark;
	LPrintln("Hour Entered: ", computer_hour);
  LMark;

	// Entering the minute
	print_module_label();
  LMark;
	LPrintln("Enter the Minute (0 ~ 59)");
  LMark;
	while(computer_min == ""){
   	LMark;
		computer_min = Serial.readStringUntil('\n');
  	LMark;
	}
	print_module_label();
  LMark;
	LPrintln("Minute Entered: ", computer_min);
  LMark;

	// Entering the second
	print_module_label();
  LMark;
	LPrintln("Enter the Second (0 ~ 59)");
  LMark;
	while(computer_sec == ""){
   	LMark;
		computer_sec = Serial.readStringUntil('\n');
  	LMark;
	}
	print_module_label();
  LMark;
	LPrintln("Second Entered: ", computer_sec);
  LMark;

	// Adjust to user input time
	_adjust(DateTime(computer_year.toInt(), computer_month.toInt(), computer_day.toInt(), computer_hour.toInt(), computer_min.toInt(), computer_sec.toInt()));
  LMark;

	print_module_label();
  LMark;
	LPrintln("Time set to user input time:");
  LMark;
	print_time();
  LMark;

	// Adjust to UTC time as default time
	convert_local_to_utc();
  LMark;

	// If local time is enable, measure local time
	if(use_local_time){
   	LMark;
		local_rtc();
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::set_rtc_to_compile_time()
{
  LMark;
	// This sets to local time zone
	_adjust( DateTime(F(__DATE__), F(__TIME__)) );
  LMark;

	print_module_label();
  LMark;
	LPrintln("Time set to compile time:");
  LMark;
	print_time();
  LMark;

	// Adjust to UTC time as default time
	convert_local_to_utc();
  LMark;

	// If local time is enable, measure local time
	if(use_local_time){
   	LMark;
		local_rtc();
  	LMark;
	}

}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::convert_local_to_utc(const bool to_utc)
{
  LMark;
	float adj = ( (to_utc) ? 1. : -1. ) * timezone_adjustment[(int)timezone];
  LMark;
	int min;
  LMark;

	if ( (adj-(int)adj) == 0 ) {
   	LMark;
		min = 0;
  	LMark;
	} else if ( (adj-(int)adj) > 0 ) {
		min = 30;
  	LMark;
	} else {
		min = -30;
  	LMark;
	}

	DateTime utc_time = now() + TimeSpan(0, (int)adj, min, 0);
  LMark;

	print_module_label();
  LMark;
	LPrintln("Adjusting time to ", (to_utc) ? "UTC" : "Local");
  LMark;

	_adjust(utc_time);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
DateTime LoomRTC::convert_daylight_to_standard(DateTime local_time){
  LMark;
	/*
	if(timezone == TimeZone::WAT || timezone == TimeZone::AT ||
	   timezone == TimeZone::HST || timezone == TimeZone::BRT ||
	   timezone == TimeZone::ZP4 || timezone == TimeZone::ZP5 ||
	   timezone == TimeZone::ZP6 || timezone == TimeZone::ZP7 ||
	   timezone == TimeZone::AWST || timezone == TimeZone::ACST ||
	   timezone == TimeZone::AEST || timezone == TimeZone::CET) LPrintln("No daylight saving or summer time in this time zone");
	*/
	if (timezone == TimeZone::ADT || timezone == TimeZone::AST ||
		timezone == TimeZone::EDT || timezone == TimeZone::EST ||
		timezone == TimeZone::CDT || timezone == TimeZone::CST ||
		timezone == TimeZone::MDT || timezone == TimeZone::MST ||
		timezone == TimeZone::PDT || timezone == TimeZone::PST ||
		timezone == TimeZone::AKDT || timezone == TimeZone::AKST){
			local_time = us_daylight_to_standard(local_time);
		}
	else if (timezone == TimeZone::BST || timezone == TimeZone::GMT ||
			 timezone == TimeZone::EEST || timezone == TimeZone::EET ){
			local_time = eu_daylight_to_standard(local_time);
		}

	return local_time;

}

///////////////////////////////////////////////////////////////////////////////
DateTime LoomRTC::us_daylight_to_standard(DateTime local_time){
  LMark;
	DateTime time = local_time;
  LMark;
	if(time.dayOfTheWeek() == 0 && time.month() == 3 &&
	   time.day() >= 8 && time.day() <= 16 && time.hour() >= 2 &&
	   time.minute() >= 0 && time.second() >= 0 && converted == false){
      LMark;
		  print_module_label();
      LMark;
		  LPrintln("Switch to Daylight Saving Time");
      LMark;
		  time = time + TimeSpan(0, 1, 0, 0);
    	LMark;
			converted = true;
      LMark;
		   	switch(timezone) {
				case TimeZone::AST :
					timezone = TimeZone::ADT;
      		LMark;
					LPrintln("Switched from AST to ADT");
      		LMark;
					break;

				case TimeZone::EST :
      		LMark;
					timezone = TimeZone::EDT;
      		LMark;
					LPrintln("Switched from EST to EDT");
      		LMark;
					break;

				case TimeZone::CST :
      		LMark;
					timezone = TimeZone::CDT;
      		LMark;
					LPrintln("Switched from CST to CDT");
      		LMark;
					break;

				case TimeZone::MST :
      		LMark;
					timezone = TimeZone::MDT;
      		LMark;
					LPrintln("Switched from MST to MDT");
      		LMark;
					break;

				case TimeZone::PST :
      		LMark;
					timezone = TimeZone::PDT;
      		LMark;
					LPrintln("Switched from PST to PDT");
      		LMark;
					break;

				case TimeZone::AKST :
      		LMark;
					timezone = TimeZone::AKDT;
      		LMark;
					LPrintln("Switched from AKST to AKDT");
      		LMark;
					break;
      		LMark;

				default:
      		LMark;
					LPrintln("If you get false time read, please submit a Github issue. If not, ignore this message.");
      		LMark;
					break;
			   }

	   }
	else if(time.dayOfTheWeek() == 0 && time.month() == 11 &&
	   		time.day() >= 1 && time.day() <= 8 && time.hour() >= 2 &&
	   		time.minute() >= 0 && time.second() >= 0 && converted == false){
     		LMark;
				print_module_label();
     		LMark;
				LPrintln("Switch to Standard Time");
     		LMark;
				time = time + TimeSpan(0, -1, 0, 0);
     		LMark;
				converted = true;
     		LMark;
				print_module_label();
     		LMark;
				switch(timezone) {
					case TimeZone::ADT :
						timezone = TimeZone::AST;
       			LMark;
						LPrintln("Switched from ADT to AST");
       			LMark;
						break;

					case TimeZone::EDT :
       			LMark;
						timezone = TimeZone::EST;
       			LMark;
						LPrintln("Switched from EDT to EST");
       			LMark;
						break;

					case TimeZone::CDT :
       			LMark;
						timezone = TimeZone::CST;
       			LMark;
						LPrintln("Switched from CDT to CST");
       			LMark;
						break;

					case TimeZone::MDT :
       			LMark;
						timezone = TimeZone::MST;
       			LMark;
						LPrintln("Switched from MDT to MST");
       			LMark;
						break;

					case TimeZone::PDT :
       			LMark;
						timezone = TimeZone::PST;
       			LMark;
						LPrintln("Switched from PDT to PST");
       			LMark;
						break;

					case TimeZone::AKDT :
       			LMark;
						timezone = TimeZone::AKST;
       			LMark;
						LPrintln("Switched from AKDT to AKST");
       			LMark;
						break;

					default:
       			LMark;
						LPrintln("If you get false time read, please submit a Github issue. If not, ignore this message.");
       			LMark;
						break;
			   }
		}
		else if ((time.dayOfTheWeek() != 0) && converted == true){
    	LMark;
			converted = false;
   		LMark;
		}
	return time;
}

///////////////////////////////////////////////////////////////////////////////
DateTime LoomRTC::eu_daylight_to_standard(DateTime local_time){
  LMark;

	DateTime time = local_time;
  LMark;
	if(time.dayOfTheWeek() == 0 && time.month() == 3 &&
	   time.day() >= 25 && time.day() <= 31 && time.hour() >= 1 &&
	   time.minute() >= 0 && time.second() >= 0 && converted == false){
      	LMark;
		   	print_module_label();
      	LMark;
		   	LPrintln("Switch to Summer Time");
      	LMark;
		   	time = time + TimeSpan(0, 1, 0, 0);
      	LMark;
		   	converted = true;
      	LMark;
		   	print_module_label();
     		LMark;
				switch(timezone) {
					case TimeZone::GMT :
       			LMark;
						timezone = TimeZone::BST;
       			LMark;
						LPrintln("Switched from GMT to BST");
       			LMark;
						break;

					case TimeZone::EET :
       			LMark;
						timezone = TimeZone::EEST;
       			LMark;
						LPrintln("Switched from EET to EEST");
       			LMark;
						break;

					default:
       			LMark;
						LPrintln("If you get false time read, please submit a Github issue. If not, ignore this message.");
       			LMark;
						break;
				}
	   }
	else if(time.dayOfTheWeek() == 0 && time.month() == 10 &&
	   		time.day() >= 25 && time.day() <= 31 && time.hour() >= 1 &&
	   		time.minute() >= 0 && time.second() >= 0 && converted == false){
				print_module_label();
     		LMark;
				LPrintln("Switch to Standard Time");
     		LMark;
				time = time + TimeSpan(0, -1, 0, 0);
     		LMark;
				converted = true;
     		LMark;
				print_module_label();
     		LMark;
				switch(timezone) {
					case TimeZone::BST :
       			LMark;
						timezone = TimeZone::GMT;
       			LMark;
						LPrintln("Switched from BST to GMT");
       			LMark;
						break;

					case TimeZone::EEST :
       			LMark;
						timezone = TimeZone::EET;
       			LMark;
						LPrintln("Switched from EEST to EET");
       			LMark;
						break;

					default:
       			LMark;
						LPrintln("If you get false time read, please submit a Github issue. If not, ignore this message.");
       			LMark;
						break;
				}
			}

	else if ((time.dayOfTheWeek() != 0) && converted == true){
   	LMark;
		converted = false;
  	LMark;
	}

	return time;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomRTC::rtc_validity_check()
{
  LMark;
	DateTime time_check = now();
  LMark;
	int y = time_check.year();
  LMark;

	// A basic validity check of date
	if ( (y < 2019) || (y > 2050)) {
   	LMark;
		print_module_label();
   	LMark;
		LPrint("RTC Time is invalid: ", y, '\n');
   	LMark;
		set_rtc_to_compile_time();
   	LMark;
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::link_device_manager(LoomManager* LM)
{
  LMark;
	LoomModule::link_device_manager(LM);
  LMark;

	if (LM) {
   	LMark;
		// Set manager's interrupt manager
		LM->rtc_module = this;
  	LMark;

		auto interrupt_manager = LM->get_interrupt_manager();
   	LMark;
		if (interrupt_manager) {
    	LMark;
			interrupt_manager->set_RTC_module( this );
   		LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::time_adjust(const DateTime time, const bool is_local)
{
  LMark;
	_adjust(time);
  LMark;

	// Check if source time is not in desired mode
	if (use_local_time != is_local) {
   	LMark;
		convert_local_to_utc(false);
  	LMark;
	}

	print_module_label();
  LMark;
	LPrint("Adjusted time to: ");
  LMark;
	print_DateTime(now());
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
