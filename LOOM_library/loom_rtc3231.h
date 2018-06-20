// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <RTClibExtended.h>

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>
// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================

// NOTE: Must include the following line in the RTClibExtended.h file to use with M0:
// #define _BV(bit) (1 << (bit))
const byte wakeUpPin = 11;

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 
struct state_rtc3231_t {
	RTC_DS3231 inst_rtc3231;
	char timestring[24];
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct state_rtc3231_t state_rtc3231;

volatile bool alarm_flag = true; // Flag is set with external Pin A0 Interrupt by RTC
volatile bool LEDState = false; // flag to toggle LED
volatile int HR = 8; // Hr of the day we want alarm to go off
volatile int MIN = 0; // Min of each hour we want alarm to go off

#if LOOM_DEBUG == 0
	volatile int WakePeriodMin = 15;  // Period of time to take sample in Min, reset alarm based on this period (Bo - 5 min)
#elif  LOOM_DEBUG == 1
	volatile int WakePeriodMin = 1;
#endif // LOOM_DEBUG

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_rtc3231();
void measure_rtc3231();
void get_timestring();
void setAlarmFunction();
void clearAlarmFunction();
void wake();

// ================================================================
// ===                          SETUP                           ===
// ================================================================


void setup_rtc3231() {
	// RTC Timer settings here
	if (! state_rtc3231.inst_rtc3231.begin()) {
	#if DEBUG == 1
		Serial.println("Couldn't find RTC");
	#endif
		while (1);
	}
	// This may end up causing a problem in practice - what if RTC looses power in field? Shouldn't happen with coin cell batt backup
	if (state_rtc3231.inst_rtc3231.lostPower()) {
		#if DEBUG == 1
			Serial.println("RTC lost power, lets set the time!");
		#endif
		// following line sets the RTC to the date & time this sketch was compiled
		state_rtc3231.inst_rtc3231.adjust(DateTime(F(__DATE__), F(__TIME__)));
	}
	//clear any pending alarms
	clearAlarmFunction();

	// Querry Time and print
	DateTime now = state_rtc3231.inst_rtc3231.now();
	#if DEBUG == 1
		Serial.print("RTC Time is: ");
		Serial.print(now.hour(), DEC); Serial.print(':'); Serial.print(now.minute(), DEC); Serial.print(':'); Serial.print(now.second(), DEC); Serial.println();
	#endif
	//Set SQW pin to OFF (in my case it was set by default to 1Hz)
	//The output of the DS3231 INT pin is connected to this pin
	//It must be connected to arduino Interrupt pin for wake-up
	state_rtc3231.inst_rtc3231.writeSqwPinMode(DS3231_OFF);

	//Set alarm1
	setAlarmFunction();
}

// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================

void measure_rtc3231() { 
	DateTime now = state_rtc3231.inst_rtc3231.now();
	state_rtc3231.month  = now.month();
	state_rtc3231.day    = now.day();
	state_rtc3231.hour   = now.hour();
	state_rtc3231.minute = now.minute();
	
	String RTC_monthString = String(state_rtc3231.month, DEC);
	String RTC_dayString   = String(state_rtc3231.day, DEC);
	String RTC_hrString    = String(state_rtc3231.hour, DEC);
	String RTC_minString   = String(state_rtc3231.minute, DEC);
	String RTC_timeString  = RTC_hrString + ":" + RTC_minString + "_" + RTC_monthString + "/" + RTC_dayString;
	
	RTC_timeString.toCharArray(state_rtc3231.timestring, 24);
}

// *********
// RTC helper function
// Function to query current RTC time and add the period to set next alarm cycle
// *********
void setAlarmFunction() {
	DateTime now = state_rtc3231.inst_rtc3231.now(); // Check the current time

	// Calculate new time
	MIN = (now.minute() + WakePeriodMin) % 60; // wrap-around using modulo every 60 sec
	HR  = (now.hour() + ((now.minute() + WakePeriodMin) / 60)) % 24; // quotient of now.min+periodMin added to now.hr, wraparound every 24hrs
	#if DEBUG == 1
		Serial.print("Resetting Alarm 1 for: "); Serial.print(HR); Serial.print(":"); Serial.println(MIN);
	#endif

	//Set alarm1
	state_rtc3231.inst_rtc3231.setAlarm(ALM1_MATCH_HOURS, MIN, HR, 0);   //set your wake-up time here
	state_rtc3231.inst_rtc3231.alarmInterrupt(1, true);
}

//*********
// RTC helper function
// When exiting the sleep mode we clear the alarm
//*********
void clearAlarmFunction() {
	//clear any pending alarms
	state_rtc3231.inst_rtc3231.armAlarm(1, false);
	state_rtc3231.inst_rtc3231.clearAlarm(1);
	state_rtc3231.inst_rtc3231.alarmInterrupt(1, false);
	state_rtc3231.inst_rtc3231.armAlarm(2, false);
	state_rtc3231.inst_rtc3231.clearAlarm(2);
	state_rtc3231.inst_rtc3231.alarmInterrupt(2, false);
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








