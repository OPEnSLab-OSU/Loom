// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <RTClibExtended.h>
// NOTE: Must include the following line in the RTClibExtended.h file to use with M0:
// #define _BV(bit) (1 << (bit))

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>
// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================


const byte wakeUpPin = 11;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


const int hour_adjustment   = 0;
const int minute_adjustment = 0;

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 
struct state_rtc_t {
	char    timestring[20], datestring[20], weekday[10];
	int     year;
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

volatile bool alarm_flag = true;  // Flag is set with external Pin A0 Interrupt by RTC
volatile bool LEDState   = false; // flag to toggle LED
volatile int  HR  = 8;            // Hr of the day we want alarm to go off
volatile int  MIN = 0;            // Min of each hour we want alarm to go off

#if LOOM_DEBUG == 0
	volatile int WakePeriodMin = 15;  // Period of time to take sample in Min, reset alarm based on this period (Bo - 5 min)
#elif  LOOM_DEBUG == 1
	volatile int WakePeriodMin = 1;
#endif // LOOM_DEBUG

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void  setup_rtc();
void  measure_rtc();
char* get_datestring();
char* get_timestring();
char* get_weekday();
#if  is_rtc3231 == 1
	void setAlarmFunction();
	void clearAlarmFunction();
	void wake();
#endif
#if LOOM_DEBUG == 1
	void print_time();
#endif

// ================================================================
// ===                          SETUP                           ===
// ================================================================


void setup_rtc() {
	if (! rtc_inst.begin()) {
		LOOM_DEBUG_Println("Couldn't find RTC");
		while (1);
	}

	// The following section checks if RTC is running, else sets 
	// the time to the time that the sketch was compiled
	#if is_rtc3231 == 1
		// This may end up causing a problem in practice - what if RTC looses power in field? Shouldn't happen with coin cell batt backup
		if (rtc_inst.lostPower()) {
			LOOM_DEBUG_Println("RTC lost power, lets set the time!");
			rtc_inst.adjust(DateTime(F(__DATE__), F(__TIME__)));
		}
	#elif is_rtc8523 == 1
		if (! rtc_inst.initialized()) {
			LOOM_DEBUG_Println("RTC is NOT running!");
			rtc_inst.adjust(DateTime(F(__DATE__), F(__TIME__)));
		}
	#endif

	//clear any pending alarms
	#if  is_rtc3231 == 1
		clearAlarmFunction();
	#endif

	// Query Time and print
	#if LOOM_DEBUG == 1
		print_time();
	#endif

	#if is_rtc3231 == 1
		//Set SQW pin to OFF (in my case it was set by default to 1Hz)
		//The output of the DS3231 INT pin is connected to this pin
		//It must be connected to arduino Interrupt pin for wake-up
		rtc_inst.writeSqwPinMode(DS3231_OFF);

		//Set alarm1
		setAlarmFunction();
	#endif	
}


// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================

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
// *********
// RTC helper function
// Function to query current RTC time and add the period to set next alarm cycle
// *********
void setAlarmFunction() {
	DateTime now = rtc_inst.now(); // Check the current time

	// Calculate new time
	MIN = (now.minute() + WakePeriodMin) % 60; // wrap-around using modulo every 60 sec
	HR  = (now.hour() + ((now.minute() + WakePeriodMin) / 60)) % 24; // quotient of now.min+periodMin added to now.hr, wraparound every 24hrs
	
	LOOM_DEBUG_println4("Resetting Alarm 1 for: ", HR, ":", MIN);

	//Set alarm1
	rtc_inst.setAlarm(ALM1_MATCH_HOURS, MIN, HR, 0);   //set your wake-up time here
	rtc_inst.alarmInterrupt(1, true);
}

//*********
// RTC helper function
// When exiting the sleep mode we clear the alarm
//*********
void clearAlarmFunction() {
	//clear any pending alarms
	rtc_inst.armAlarm(1, false);
	rtc_inst.clearAlarm(1);
	rtc_inst.alarmInterrupt(1, false);
	rtc_inst.armAlarm(2, false);
	rtc_inst.clearAlarm(2);
	rtc_inst.alarmInterrupt(2, false);
}

void wake() {
	alarm_flag = true;
	#ifdef is_m0
	detachInterrupt(digitalPinToInterrupt(wakeUpPin));
	#endif //is_M0

	#ifdef is_32u4
	disableInterrupt(wakeUpPin);
	#endif //is_32U4
}
#endif // of is_rtc3231 == 1



#if LOOM_DEBUG == 1
void print_time()
{
	measure_rtc();
	LOOM_DEBUG_Println2("Date: ", get_datestring());
	LOOM_DEBUG_Println2("Time: ", get_timestring());
	LOOM_DEBUG_Println2("Day : ", get_weekday());
}
#endif






