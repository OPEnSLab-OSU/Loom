// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <RTClibExtended.h>
// NOTE: Must include the following line in the RTClibExtended.h file to use with M0:
// #define _BV(bit) (1 << (bit))


#if is_rtc3231 == 1
	#define EI_NOTEXTERNAL
	#include <EnableInterrupt.h>
#endif

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================

const float timezone_adjustment[34] =
{
	1  /* WAT */, 2    /* AT  */, 3     /* ADT */, 4   /* AST */, 4   /* EDT */, 5  /* EST */, 5  /* CDT */,
	6  /* CST */, 6    /* MDT */, 7     /* MST */, 7   /* PDT */, 8   /* PST */, 8  /* ALDT*/, 9  /* ALST*/,
	10 /* HST */, 11   /* SST */, 0     /* GMT */, -1  /* BST */, -1  /* CET */, -2 /* CEST*/, -2 /* EET */,
	-3 /* EEST*/, -3   /* BT  */, -4    /* ZP4 */, -5  /* ZP5 */, -6  /* ZP6 */, -7 /* ZP7 */, -8 /* AWST*/,
	-9 /* AWDT*/, -9.5 /* ACST*/, -10.5 /* ACDT*/, -10 /* AEST*/, -11 /* AEDT*/
};

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 
struct state_rtc_t {
	char    timestring[20], datestring[20], weekday[10];
	uint     year;
	uint8_t month, day, hour, minute, second;
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

struct state_rtc_t state_rtc;

// Select correct RTC object based on which is being used
#if  is_rtc3231 == 1
	RTC_DS3231  rtc_inst;
#elif is_rtc8523 == 1
	RTC_PCF8523 rtc_inst;
#endif


	// Some of these should be deleted 

volatile bool alarm_flag = true;  // Flag is set with external Pin A0 Interrupt by RTC
volatile bool LEDState   = false; // flag to toggle LED

// #if LOOM_DEBUG == 0
// 	volatile int WakePeriodMin = 15;  // Period of time to take sample in Min, reset alarm based on this period (Bo - 5 min)
// #elif  LOOM_DEBUG == 1
// 	volatile int WakePeriodMin = 1;
// #endif // LOOM_DEBUG

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void  setup_rtc();
void  set_rtc_to_compile_time();
bool  set_rtc_from_internet_time();
void  convert_local_to_utc(bool to_utc);
void  convert_local_to_utc();
bool  rtc_validity_check();
void  measure_rtc();
char* get_datestring(); 
char* get_timestring();
char* get_weekday();

#if  is_rtc3231 == 1
	void setAlarmFunction();
	void clearRTCAlarms();
	void rtc_interrupt_reset();
	void wake_RTC_ISR();
#endif

void  print_time();
void  print_DateTime(DateTime time);


// ================================================================
// ===                          SETUP                           ===
// ================================================================


void setup_rtc() {
	if (!rtc_inst.begin()) {
		LOOM_DEBUG_Println("Couldn't find RTC");
		while (1);
	}

	LOOM_DEBUG_Println("\nCurrent Time (before possible resetting)");
	print_time();


	bool internet_time_success = false;


	// Try to set the time from internet
	#if get_time_from_internet == 1 
		internet_time_success = set_rtc_from_internet_time();
	#endif 

	// If unable to set time via internet, default to normal behavior
	if (!internet_time_success) {

		// The following section checks if RTC is running, else sets 
		// the time to the time that the sketch was compiled
		#if is_rtc3231 == 1
			if (rtc_inst.lostPower()) {
				LOOM_DEBUG_Println("RTC 3231 lost power");
				set_rtc_to_compile_time();
			}
		#elif is_rtc8523 == 1
			if (!rtc_inst.initialized()) {
				LOOM_DEBUG_Println("RTC 8523 was not initialized");
				set_rtc_to_compile_time();
			}
		#endif

		// Make sure the RTC time is even valid, if not, set to compile time
		rtc_validity_check();

	} // of if (!internet_time_success)


	// Alarm setup
	#if  is_rtc3231 == 1
		clearRTCAlarms(); 	//clear any pending alarms

		//Set SQW pin to OFF (in my case it was set by default to 1Hz)
		//The output of the DS3231 INT pin is connected to this pin
		rtc_inst.writeSqwPinMode(DS3231_OFF);

		// Configure RTC Interrupt pin to be input
		pinMode(RTC_pin, INPUT_PULLUP);
	#endif	


	// Query Time and print
	print_time();
}


// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================


// --- SET RTC TO COMPILE TIME ---
//
// Sets the RTC clock to compile time
//
void set_rtc_to_compile_time()
{
	// This sets to local time zone
	rtc_inst.adjust(DateTime(F(__DATE__), F(__TIME__)));
	LOOM_DEBUG_Println("Time set to compile time:");
	print_time();

	// Adjust to UTC time if enabled
	#if use_utc_time == 1
		convert_local_to_utc();
	#endif
}



// --- SET RTC FROM INTERNET TIME ---
//
// Attempt to set the RTC from time obtained over internet
//
// @return  True if RTC successfully set from internet time
//
bool set_rtc_from_internet_time()
{
	uint32_t unixTime = 0;
	bool internet_time_success = false;

	#if is_ethernet == 1  
		unixTime = get_time_ethernet();
	#elif is_wifi == 1
		unixTime = get_time_wifi();
	#endif

	LOOM_DEBUG_Println2("UNIX TIME: ", unixTime);

	if (unixTime != 0) {
		// Set to UTC time
		rtc_inst.adjust(DateTime(unixTime));

		LOOM_DEBUG_Println("\nTime set to:");
		print_time();

		// If not using UTC Time convert to local
		#if use_utc_time != 1
			convert_local_to_utc(false);
		#endif

		internet_time_success = true;
	} 
	
	LOOM_DEBUG_Println3("Set time from internet ", (internet_time_success) ? "successful" : "failed", "\n");

	return internet_time_success;
}



// --- CONVERT LOCAL TIME TO UTC TIME ---
//
// Converts a time assumed to be local (no checks provided)
// to UTC time
//
// @param to_utc  True to convert local to UTC, false to convert UTC to local
// 
void convert_local_to_utc(bool to_utc) 
{
	float adj = ( (to_utc) ? 1. : -1. ) * timezone_adjustment[timezone];
	int min;

	if ( (adj-(int)adj) == 0 ) { min = 0;
	} else if ( (adj-(int)adj) > 0 ) { min = 30;
	} else { min = -30; }

	DateTime utc = rtc_inst.now() + TimeSpan(0, (int)adj, min, 0);
	rtc_inst.adjust(utc);

	LOOM_DEBUG_Println3("Time adjusted to ", (to_utc) ? "UTC" : "Local" , " time:");
	print_time();
	LOOM_DEBUG_Println();
}

// Default behavior - convert local to UTC
void convert_local_to_utc() 
{ 
	convert_local_to_utc(true); 
}



// --- RTC VALIDITY CHECK ---
//
// Make sure the RTC time is even valid
// If not, set to compile time
//
// @return True if RTC time appeared to be valid (not necessarily correct)
//
bool rtc_validity_check()
{
	DateTime time_check = rtc_inst.now();
	int y = time_check.year();
	int m = time_check.month();
	int d = time_check.day();

	// A basic validity check of date
	if ( (y < 2018) || (y > 2050) || (m < 1) || (m > 12) || (d < 1) || (d > 31) ) {
		LOOM_DEBUG_Println("RTC Time is invalid");
		set_rtc_to_compile_time();
		return false;
	}

	return true;
}





// --- MEASURE RTC ---
//
// Reads the RTC time and copies to the state struct
//
void measure_rtc() { 
	DateTime now = rtc_inst.now();

	state_rtc.year   = now.year();
	state_rtc.month  = now.month();
	state_rtc.day    = now.day();
	state_rtc.hour   = now.hour();
	state_rtc.minute = now.minute();
	state_rtc.second = now.second();

	sprintf(state_rtc.weekday, "%s", daysOfTheWeek[now.dayOfTheWeek()]);
	
	String(String(state_rtc.year,   DEC) + '/' 
	     + String(state_rtc.month,  DEC) + '/' 
	     + String(state_rtc.day,    DEC))
	     .toCharArray(state_rtc.datestring, 20);

	String(String(state_rtc.hour,   DEC) + ':' 
	     + String(state_rtc.minute, DEC) + ':' 
	     + String(state_rtc.second, DEC))
	     .toCharArray(state_rtc.timestring, 20);
}



char* get_datestring() {
	measure_rtc();
	return state_rtc.datestring;
}

char* get_timestring() {
	measure_rtc();
	return state_rtc.timestring;
}

char* get_weekday() {
	measure_rtc();
	return state_rtc.weekday;
}



#if  is_rtc3231 == 1

// --- SET RTC ALARM RELATIVE
void setRTCAlarm_Relative(int hours, int minutes, int seconds)
{
	// Display current time
	LOOM_DEBUG_Println("Current Time:");
	print_DateTime( rtc_inst.now() );

	// Calculate new time
	DateTime future( rtc_inst.now() + TimeSpan(0,hours,minutes,seconds) );
	LOOM_DEBUG_Println("Resetting Alarm 1 for:"); 
	print_DateTime(future);

	// Set alarm 1																	// not sure if this 0 is correct
	rtc_inst.setAlarm(ALM1_MATCH_HOURS, future.second(), future.minute(), future.hour(), 0); 
	rtc_inst.alarmInterrupt(1, true);
}


// --- SET RTC ALARM RELATIVE
void setRTCAlarm_Absolute(int hour, int minute, int second)
{
	DateTime now = rtc_inst.now();

	// Display current time
	LOOM_DEBUG_Println("Current Time:");
	print_DateTime( now );

	// Calculate new time (checking to make sure alarm is not set in past)
	DateTime future = DateTime(now.year(), now.month(), now.day(), hour, minute, second);

	// If that time has already passed on current day, set for next day
	// Using Timespan of 24 hours to prevent errors
	if ( (future - now).totalseconds() < 0 ) {
		LOOM_DEBUG_Println("Time already passed, setting for next day");
		future = future + TimeSpan(1,0,0,0);
	} 

	LOOM_DEBUG_Println("Resetting Alarm 1 for:"); 
	print_DateTime(future);

	// Set alarm 1																	// not sure if this 0 is correct
	rtc_inst.setAlarm(ALM1_MATCH_HOURS, future.second(), future.minute(), future.hour(), 0); 	    								
	rtc_inst.alarmInterrupt(1, true);
}



// --- CLEAR RTC ALARMS ---
//
// Clear any pending alarms on the RTC
//
void clearRTCAlarms() {
	rtc_inst.armAlarm(1, false);
	rtc_inst.clearAlarm(1);
	rtc_inst.alarmInterrupt(1, false);
	rtc_inst.armAlarm(2, false);
	rtc_inst.clearAlarm(2);
	rtc_inst.alarmInterrupt(2, false);
}



void rtc_interrupt_reset()
{
	// Clears any interrupts that may be pending on M0
	detachInterrupt(digitalPinToInterrupt(RTC_pin));
	delay(20);
	attachInterrupt(digitalPinToInterrupt(RTC_pin), wake_RTC_ISR, LOW);
}


// --- WAKE RTC ISR ---
//
// The interrupt service routine called when a device 
// wakes from sleep via an RTC interrupt
//
void wake_RTC_ISR()
{
	detachInterrupt(digitalPinToInterrupt(RTC_pin));
}

#endif // of is_rtc3231 == 1



// --- PRINT TIME ---
//
// Prints the time according to the RTC clock
//
void print_time()
{
	measure_rtc();
	LOOM_DEBUG_Println("Time:");
	LOOM_DEBUG_Println2("\tDate: ", get_datestring());
	LOOM_DEBUG_Println2("\tTime: ", get_timestring());
	LOOM_DEBUG_Println2("\tDay : ", get_weekday());
	LOOM_DEBUG_Println();
}


// --- PRINT DATETIME ---
//
// Prints a DateTime object
//
// @param time  The DateTime object to print
//
void print_DateTime(DateTime time) 
{
	LOOM_DEBUG_Println("DateTime:");
	LOOM_DEBUG_Print(time.year());   LOOM_DEBUG_Print('/');
	LOOM_DEBUG_Print(time.month());  LOOM_DEBUG_Print('/');
	LOOM_DEBUG_Print(time.day());    LOOM_DEBUG_Print(' ');
	LOOM_DEBUG_Print(time.hour());   LOOM_DEBUG_Print(':');
	LOOM_DEBUG_Print(time.minute()); LOOM_DEBUG_Print(':');
	LOOM_DEBUG_Print(time.second()); LOOM_DEBUG_Println();
}


