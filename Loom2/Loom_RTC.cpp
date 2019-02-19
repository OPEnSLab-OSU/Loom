
#include "Loom_RTC.h"
#include "Loom_Manager.h"


// const char* LoomRTC::daysOfTheWeek[] = 
// 	{"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


// const float LoomRTC::timezone_adjustment[] =
// 	{
// 		1  /* WAT */, 2    /* AT  */, 3     /* ADT */, 4   /* AST */, 4   /* EDT */, 5  /* EST */, 5  /* CDT */,
// 		6  /* CST */, 6    /* MDT */, 7     /* MST */, 7   /* PDT */, 8   /* PST */, 8  /* ALDT*/, 9  /* ALST*/,
// 		10 /* HST */, 11   /* SST */, 0     /* GMT */, -1  /* BST */, -1  /* CET */, -2 /* CEST*/, -2 /* EET */,
// 		-3 /* EEST*/, -3   /* BT  */, -4    /* ZP4 */, -5  /* ZP5 */, -6  /* ZP6 */, -7 /* ZP7 */, -8 /* AWST*/,
// 		-9 /* AWDT*/, -9.5 /* ACST*/, -10.5 /* ACDT*/, -10 /* AEST*/, -11 /* AEDT*/
// 	};


char* LoomRTC::enum_timezone_string(TimeZone t)
{
	switch(t) {
		case WAT  : return "WAT";
		case AT   : return "AT";
		case ADT  : return "ADT";
		case AST  : return "AST";
		case EDT  : return "EDT";
		case EST  : return "EST";
		case CDT  : return "CDT";
		case CST  : return "CST";
		case MDT  : return "MDT";
		case MST  : return "MST";
		case PDT  : return "PDT";
		case PST  : return "PST";
		case ALDT : return "ALDT";
		case ALST : return "ALST";
		case HST  : return "HST";
		case SST  : return "SST";
		case GMT  : return "GMT";
		case BST  : return "BST";
		case CET  : return "CET";
		case CEST : return "CEST";
		case EET  : return "EET";
		case EEST : return "EEST";
		case BT   : return "BT";
		case ZP4  : return "ZP4";
		case ZP5  : return "ZP5";
		case ZP6  : return "ZP6";
		case ZP7  : return "ZP7";
		case AWST : return "AWST";
		case AWDT : return "AWDT";
		case ACST : return "ACST";
		case ACDT : return "ACDT";
		case AEST : return "AEST";
		case AEDT : return "AEDT";
	}
}



// --- CONSTRUCTOR ---
LoomRTC::LoomRTC(	char* 	module_name,
	 
					TimeZone 	timezone,
					bool 		use_utc_time,
					bool 		get_internet_time

	) : LoomModule( module_name )
{
	this->timezone			= timezone;
	this->use_utc_time 		= use_utc_time;
	this->get_internet_time = get_internet_time;
}

// --- DESTRUCTOR ---
LoomRTC::~LoomRTC() 
{

}


// --- PUBLIC METHODS ---

void LoomRTC::print_config()
{
	LoomModule::print_config();

	Println3('\t', "Use UTC Time        : ", use_utc_time);
	Println3('\t', "Get Internet Time   : ", get_internet_time);
}


void LoomRTC::print_time()
{
	// measure();
	print_module_label();

	Println("Time:");
	// char buf[20];
	// get_datestring(buf);
	// Println2("\tDate: ", buf);
	// get_timestring(buf);
	// Println2("\tTime: ", buf);
	// Println2("\tDay : ", get_weekday());
	// Println();


	Println2("\tDate: ", get_datestring());
	Println2("\tTime: ", get_timestring());
	Println2("\tDay : ", get_weekday());
	Println();
}


void LoomRTC::print_DateTime(DateTime time) 
{
	Println("DateTime:");
	Print(time.year());   Print('/');
	Print(time.month());  Print('/');
	Print(time.day());    Print(' ');
	Print(time.hour());   Print(':');
	Print(time.minute()); Print(':');
	Print(time.second()); Println();
}


void LoomRTC::read_rtc()
{
	get_datestring();
	get_timestring();
}


// void LoomRTC::measure()
// {
// 	get_datestring();
// 	get_timestring();
// }


void LoomRTC::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	measure();
	append_to_bundle(bndl, id_prefix, "DateString", datestring, NEW_MSG);
	append_to_bundle(bndl, id_prefix, "TimeString", timestring);
}


char* LoomRTC::get_datestring()
{
	DateTime time = now();
	sprintf(datestring, "%d/%d/%d", time.year(), time.month(), time.day() );
	return datestring;
} 

void LoomRTC::get_datestring(char* buf)
{
	DateTime time = now();
	sprintf(buf, "%d/%d/%d", time.year(), time.month(), time.day() );
	strcpy(datestring, buf);
} 


char* LoomRTC::get_timestring()
{
	DateTime time = now();
	sprintf(timestring, "%d:%d:%d", time.hour(), time.minute(), time.second() );
	return timestring;
}


void LoomRTC::get_timestring(char* buf)
{
	DateTime time = now();
	sprintf(buf, "%d:%d:%d", time.hour(), time.minute(), time.second() );
	strcpy(timestring, buf);
}


char* LoomRTC::get_weekday()
{
	return (char*)daysOfTheWeek[ now().dayOfTheWeek() ];
}


void LoomRTC::get_weekday(char* buf)
{
	strcpy( buf, daysOfTheWeek[ now().dayOfTheWeek() ] );
}



//   0: no timestamp added
//   1: only date added
//   2: only time added
//   3: both date and time added (two fields)
//   4: both date and time added (combined field)
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


void LoomRTC::convert_local_to_utc(bool to_utc)
{
	float adj = ( (to_utc) ? 1. : -1. ) * timezone_adjustment[timezone];
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




