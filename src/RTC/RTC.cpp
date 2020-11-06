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
	LoomModule::print_config();
	LPrintln("\tUse UTC Time      : ", use_local_time);
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
	if (use_local_time){
		local_rtc();
		JsonObject data = get_module_data_object(json, "LocalTime");
		sprintf(local_datestring, "%d/%d/%d", local_time.year(), local_time.month(), local_time.day());
		sprintf(local_timestring, "%d:%d:%d", local_time.hour(), local_time.minute(), local_time.second());
		data["Date"] = local_datestring;
		data["Time"] = local_timestring;
		data["TimeZone"] = enum_timezone_string(timezone);
	}
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
void LoomRTC::local_rtc(){
	local_time = now();
	float adj = -1. * timezone_adjustment[(int)timezone];
	int min;

	if ( (adj-(int)adj) == 0 ) { 
		min = 0;
	} else if ( (adj-(int)adj) > 0 ) { 
		min = 30;
	} else { 
		min = -30; 
	}

	local_time = local_time + TimeSpan(0, (int)adj, min, 0);
	local_time = convert_daylight_to_standard(local_time);
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
		if(Serial && custom_time){
			set_rtc_to_computer_time();
		}
		else{
			set_rtc_to_compile_time();
		}
	}

	// Make sure the RTC time is even valid, if not, set to compile time
	rtc_validity_check();

	// Query Time and print
	print_time();
}
///////////////////////////////////////////////////////////////////////////////
void LoomRTC::set_rtc_to_computer_time()
{

	// initialized variable for user input
	String computer_year = "";
	String computer_month = "";
	String computer_day = "";
	String computer_hour = "";
	String computer_min = "";
	String computer_sec = "";

	// Let the user know that they should enter local time
	print_module_label();
	LPrintln("Please use your local time, not UTC!");

	// Entering the year
	print_module_label();
	LPrintln("Enter the Year");
	while(computer_year == ""){
		computer_year = Serial.readStringUntil('\n');
	}
	print_module_label();
	LPrintln("Year Entered: ", computer_year);
	
	// Entering the month
	print_module_label();
	LPrintln("Enter the Month");
	while(computer_month == ""){
		computer_month = Serial.readStringUntil('\n');
	}
	print_module_label();
	LPrintln("Month Entered: ", computer_month);

	// Entering the day
	print_module_label();
	LPrintln("Enter the Day");
	while(computer_day  == ""){
		computer_day = Serial.readStringUntil('\n');
	}
	print_module_label();
	LPrintln("Day Entered: ", computer_day);

	// Entering the hour
	print_module_label();
	LPrintln("Enter the Hour (0 ~ 23)");
	while(computer_hour == ""){
		computer_hour = Serial.readStringUntil('\n');
	}
	print_module_label();
	LPrintln("Hour Entered: ", computer_hour);

	// Entering the minute
	print_module_label();
	LPrintln("Enter the Minute (0 ~ 59)");
	while(computer_min == ""){
		computer_min = Serial.readStringUntil('\n');
	}
	print_module_label();
	LPrintln("Minute Entered: ", computer_min);

	// Entering the second
	print_module_label();
	LPrintln("Enter the Second (0 ~ 59)");
	while(computer_sec == ""){
		computer_sec = Serial.readStringUntil('\n');
	}
	print_module_label();
	LPrintln("Second Entered: ", computer_sec);

	// Adjust to user input time
	_adjust(DateTime(computer_year.toInt(), computer_month.toInt(), computer_day.toInt(), computer_hour.toInt(), computer_min.toInt(), computer_sec.toInt()));
		
	print_module_label();
	LPrintln("Time set to user input time:");
	print_time();

	// Adjust to UTC time as default time
	convert_local_to_utc();

	// If local time is enable, measure local time 
	if(use_local_time){
		local_rtc();
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomRTC::set_rtc_to_compile_time()
{
	// This sets to local time zone
	_adjust( DateTime(F(__DATE__), F(__TIME__)) );
	
	print_module_label();
	LPrintln("Time set to compile time:");
	print_time();

	// Adjust to UTC time as default time
	convert_local_to_utc();
	
	// If local time is enable, measure local time 
	if(use_local_time){
		local_rtc();
	}
		
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
DateTime LoomRTC::convert_daylight_to_standard(DateTime local_time){
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
	DateTime time = local_time;
	if(time.dayOfTheWeek() == 0 && time.month() == 3 && 
	   time.day() >= 8 && time.day() <= 16 && time.hour() >= 2 && 
	   time.minute() >= 0 && time.second() >= 0 && converted == false){
		   print_module_label();
		   LPrintln("Switch to Daylight Saving Time");
		   time = time + TimeSpan(0, 1, 0, 0);
			converted = true;
		   	switch(timezone) {
				case TimeZone::AST :
					timezone = TimeZone::ADT;
					LPrintln("Switched from AST to ADT");
					break;
				
				case TimeZone::EST :
					timezone = TimeZone::EDT;
					LPrintln("Switched from EST to EDT");
					break;

				case TimeZone::CST :
					timezone = TimeZone::CDT;
					LPrintln("Switched from CST to CDT");
					break;

				case TimeZone::MST :
					timezone = TimeZone::MDT;
					LPrintln("Switched from MST to MDT");
					break;

				case TimeZone::PST :
					timezone = TimeZone::PDT;
					LPrintln("Switched from PST to PDT");
					break;

				case TimeZone::AKST :
					timezone = TimeZone::AKDT;
					LPrintln("Switched from AKST to AKDT");
					break;
				
				default:
					LPrintln("If you get false time read, please submit a Github issue. If not, ignore this message.");
					break;
			   }
		
	   }
	else if(time.dayOfTheWeek() == 0 && time.month() == 11 && 
	   		time.day() >= 1 && time.day() <= 8 && time.hour() >= 2 && 
	   		time.minute() >= 0 && time.second() >= 0 && converted == false){
				print_module_label();
				LPrintln("Switch to Standard Time");
				time = time + TimeSpan(0, -1, 0, 0);
				converted = true;
				print_module_label();			
				switch(timezone) {
					case TimeZone::ADT :
						timezone = TimeZone::AST;
						LPrintln("Switched from ADT to AST");
						break;
				
					case TimeZone::EDT :
						timezone = TimeZone::EST;
						LPrintln("Switched from EDT to EST");
						break;

					case TimeZone::CDT :
						timezone = TimeZone::CST;
						LPrintln("Switched from CDT to CST");
						break;

					case TimeZone::MDT :
						timezone = TimeZone::MST;
						LPrintln("Switched from MDT to MST");
						break;

					case TimeZone::PDT :
						timezone = TimeZone::PST;
						LPrintln("Switched from PDT to PST");
						break;

					case TimeZone::AKDT :
						timezone = TimeZone::AKST;
						LPrintln("Switched from AKDT to AKST");
						break;
					
					default:
						LPrintln("If you get false time read, please submit a Github issue. If not, ignore this message.");
						break;
			   }
		}
		else if ((time.dayOfTheWeek() != 0) && converted == true){
			converted = false;
		}
	return time;
}

///////////////////////////////////////////////////////////////////////////////
DateTime LoomRTC::eu_daylight_to_standard(DateTime local_time){

	DateTime time = local_time;
	if(time.dayOfTheWeek() == 0 && time.month() == 3 && 
	   time.day() >= 25 && time.day() <= 31 && time.hour() >= 1 && 
	   time.minute() >= 0 && time.second() >= 0 && converted == false){
		   print_module_label();
		   LPrintln("Switch to Summer Time");
		   time = time + TimeSpan(0, 1, 0, 0);
		   converted = true;
		   print_module_label();
				switch(timezone) {
					case TimeZone::GMT :
						timezone = TimeZone::BST;
						LPrintln("Switched from GMT to BST");
						break;

					case TimeZone::EET :
						timezone = TimeZone::EEST;
						LPrintln("Switched from EET to EEST");
						break;

					default:
						LPrintln("If you get false time read, please submit a Github issue. If not, ignore this message.");
						break;
				}
	   }
	else if(time.dayOfTheWeek() == 0 && time.month() == 10 && 
	   		time.day() >= 25 && time.day() <= 31 && time.hour() >= 1 && 
	   		time.minute() >= 0 && time.second() >= 0 && converted == false){
				print_module_label();
				LPrintln("Switch to Standard Time");
				time = time + TimeSpan(0, -1, 0, 0);
				converted = true;
				print_module_label();
				switch(timezone) {
					case TimeZone::BST :
						timezone = TimeZone::GMT;
						LPrintln("Switched from BST to GMT");
						break;

					case TimeZone::EEST :
						timezone = TimeZone::EET;
						LPrintln("Switched from EEST to EET");
						break;
				
					default:
						LPrintln("If you get false time read, please submit a Github issue. If not, ignore this message.");		
						break;		
				}
			}
	
	else if ((time.dayOfTheWeek() != 0) && converted == true){
		converted = false;
	}

	return time;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomRTC::rtc_validity_check()
{
	DateTime time_check = now();
	int y = time_check.year();

	// A basic validity check of date
	if ( (y < 2019) || (y > 2050)) {
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
void LoomRTC::time_adjust(const DateTime time, const bool is_local)
{
	_adjust(time);

	// Check if source time is not in desired mode
	if (use_local_time != is_local) {
		convert_local_to_utc(false);
	}

	print_module_label();
	LPrint("Adjusted time to: "); 
	print_DateTime(now());	
}

///////////////////////////////////////////////////////////////////////////////

