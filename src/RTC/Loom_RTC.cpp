
#include "Loom_RTC.h"
#include "Loom_Manager.h"


#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>



byte LoomRTC::int_pin = 0; 

/////////////////////////////////////////////////////////////////////
const char* LoomRTC::daysOfTheWeek[] = 
	{"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

/////////////////////////////////////////////////////////////////////
const float LoomRTC::timezone_adjustment[] =
	{
		1  /* WAT */, 2    /* AT  */, 3     /* ADT */, 4   /* AST */, 4   /* EDT */, 5  /* EST */, 5  /* CDT */,
		6  /* CST */, 6    /* MDT */, 7     /* MST */, 7   /* PDT */, 8   /* PST */, 8  /* ALDT*/, 9  /* ALST*/,
		10 /* HST */, 11   /* SST */, 0     /* GMT */, -1  /* BST */, -1  /* CET */, -2 /* CEST*/, -2 /* EET */,
		-3 /* EEST*/, -3   /* BT  */, -4    /* ZP4 */, -5  /* ZP5 */, -6  /* ZP6 */, -7 /* ZP7 */, -8 /* AWST*/,
		-9 /* AWDT*/, -9.5 /* ACST*/, -10.5 /* ACDT*/, -10 /* AEST*/, -11 /* AEDT*/
	};

/////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
LoomRTC::LoomRTC(	
		const char*		module_name,

		TimeZone		timezone,
		bool			use_utc_time,
		bool			get_internet_time,	

		byte			interrupt_pin
	) 
	: LoomModule( module_name )
{
	
	this->timezone			= timezone;
	this->use_utc_time 		= use_utc_time;
	this->get_internet_time = get_internet_time;

	int_pin					= interrupt_pin;
	// RTC_Int_Pin 			= int_pin; 		// This is static so ISR can be static (currently int_pin was made static instead)


	if (int_pin > 0) {
		pinMode(int_pin, INPUT_PULLUP);		
	}

	clear_alarms();
}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
LoomRTC::~LoomRTC() 
{

}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void LoomRTC::print_config()
{
	LoomModule::print_config();

	Println3('\t', "Interrupt Pin       : ", int_pin);
	Println3('\t', "Use UTC Time        : ", use_utc_time);
	Println3('\t', "Get Internet Time   : ", get_internet_time);
}

/////////////////////////////////////////////////////////////////////
void LoomRTC::print_state()
{
	print_time();
}

/////////////////////////////////////////////////////////////////////
void LoomRTC::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	read_rtc();
	append_to_bundle(bndl, id_prefix, "DateString", datestring, NEW_MSG);
	append_to_bundle(bndl, id_prefix, "TimeString", timestring);
}

/////////////////////////////////////////////////////////////////////
void LoomRTC::print_time(bool verbose)
{
	read_rtc();
	print_module_label();

	if (verbose) {
		Println("Time : ");
		Println2("\tDate: ", get_datestring());
		Println2("\tTime: ", get_timestring());
		Println2("\tDay : ", get_weekday());
		Println();
	} else {
		print_DateTime( now() );
	}
}

/////////////////////////////////////////////////////////////////////
void LoomRTC::print_DateTime(DateTime time) 
{
	// Print("DateTime:");
	Print(time.year());   Print('/');
	Print(time.month());  Print('/');
	Print(time.day());    Print(' ');
	Print(time.hour());   Print(':');
	Print(time.minute()); Print(':');
	Print(time.second()); 
	Println();
}

/////////////////////////////////////////////////////////////////////
void LoomRTC::read_rtc()
{
	get_datestring();
	get_timestring();
}

/////////////////////////////////////////////////////////////////////
char* LoomRTC::get_datestring()
{
	DateTime time = now();
	sprintf(datestring, "%d/%d/%d", time.year(), time.month(), time.day() );
	return datestring;
} 

/////////////////////////////////////////////////////////////////////
void LoomRTC::get_datestring(char* buf)
{
	DateTime time = now();
	sprintf(buf, "%d/%d/%d", time.year(), time.month(), time.day() );
	strcpy(datestring, buf);
} 

/////////////////////////////////////////////////////////////////////
char* LoomRTC::get_timestring()
{
	DateTime time = now();
	sprintf(timestring, "%d:%d:%d", time.hour(), time.minute(), time.second() );
	return timestring;
}

/////////////////////////////////////////////////////////////////////
void LoomRTC::get_timestring(char* buf)
{
	DateTime time = now();
	sprintf(buf, "%d:%d:%d", time.hour(), time.minute(), time.second() );
	strcpy(timestring, buf);
}

/////////////////////////////////////////////////////////////////////
char* LoomRTC::get_weekday()
{
	return (char*)daysOfTheWeek[ now().dayOfTheWeek() ];
}

/////////////////////////////////////////////////////////////////////
void LoomRTC::get_weekday(char* buf)
{
	strcpy( buf, daysOfTheWeek[ now().dayOfTheWeek() ] );
}

/////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////
void LoomRTC::init()
{
	if (!_begin()) {
		print_module_label();
		Println("Couldn't find RTC");
		return;
	}

	print_module_label();
	Println("Current Time (before possible resetting)");
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
		if (!_initialized()) {
			print_module_label();
			Println("RTC was not initialized");
			set_rtc_to_compile_time();
		}

		// Make sure the RTC time is even valid, if not, set to compile time
		rtc_validity_check();

	} // of if (!internet_time_success)

	// Query Time and print
	print_time();
}

/////////////////////////////////////////////////////////////////////
void LoomRTC::set_rtc_to_compile_time()
{
	// This sets to local time zone
	time_adjust( DateTime(F(__DATE__), F(__TIME__)) );
	
	print_module_label();
	Println("Time set to compile time:");
	print_time();

	// Adjust to UTC time if enabled
	if (use_utc_time) convert_local_to_utc();
}

/////////////////////////////////////////////////////////////////////
bool LoomRTC::set_rtc_from_internet_time()
{
	uint32_t unixTime = 0;
	bool internet_time_success = false;

	// #if is_ethernet == 1  
	// 	unixTime = get_time_ethernet();
	// #elif is_wifi == 1
	// 	unixTime = get_time_wifi();
	// #endif

	print_module_label();
	Println2("UNIX TIME: ", unixTime);

	if (unixTime != 0) {
		// Set to UTC time
		time_adjust(DateTime(unixTime));

		print_module_label();
		Println("\nTime set to:");
		print_time();

		// If not using UTC Time convert to local
		if (!use_utc_time) convert_local_to_utc(false);

		internet_time_success = true;
	} 

	print_module_label();
	Println3("Set time from internet ", (internet_time_success) ? "successful" : "failed", "\n");

	return internet_time_success;
} 

/////////////////////////////////////////////////////////////////////
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
	Println3("Time adjusted to ", (to_utc) ? "UTC" : "Local" , " time:");
	print_time();
	Println();
}

/////////////////////////////////////////////////////////////////////
bool LoomRTC::rtc_validity_check()
{
	DateTime time_check = now();
	int y = time_check.year();
	int m = time_check.month();
	int d = time_check.day();

	// A basic validity check of date
	if ( (y < 2018) || (y > 2050) || (m < 1) || (m > 12) || (d < 1) || (d > 31) ) {
		print_module_label();
		Println("RTC Time is invalid");
		set_rtc_to_compile_time();
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
byte LoomRTC::get_interrupt_pin()
{
	return int_pin;
	// return RTC_Int_Pin;
}

/////////////////////////////////////////////////////////////////////
void LoomRTC::RTC_Wake_ISR()
{
	// Detach interrupt to prevent duplicate triggering
	detachInterrupt(digitalPinToInterrupt(int_pin));

	// Nothing else to do because interrupt woke device
}

