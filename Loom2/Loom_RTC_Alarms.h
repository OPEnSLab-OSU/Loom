
#ifndef LOOM_RTC_ALARMS_h
#define LOOM_RTC_ALARMS_h


#include <RTClibExtended.h>
// NOTE: Must include the following line in the RTClibExtended.h file to use with M0:
// #define _BV(bit) (1 << (bit))

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>



class LoomRTC_Alarms
{


protected:

	byte int_pin;

public:

	// --- CONSTRUCTOR ---
	LoomRTC_Alarms(byte int_pin);

	// --- DESTRUCTOR ---
	virtual ~LoomRTC_Alarms();

	virtual void clear_alarms() = 0;
	virtual void set_alarm(DateTime future) = 0;


protected:


};


#endif // of #ifndef LOOM_RTC_ALARMS_h





// // --- SET RTC ALARM RELATIVE
// void setRTCAlarm_Relative(int hours, int minutes, int seconds)
// {
// 	// Display current time
// 	LOOM_DEBUG_Println("Current Time:");
// 	print_DateTime( rtc_inst.now() );

// 	// Calculate new time
// 	DateTime future( rtc_inst.now() + TimeSpan(0,hours,minutes,seconds) );
// 	LOOM_DEBUG_Println("Resetting Alarm 1 for:"); 
// 	print_DateTime(future);

// 	// Set alarm 1																	// not sure if this 0 is correct
// 	rtc_inst.setAlarm(ALM1_MATCH_HOURS, future.second(), future.minute(), future.hour(), 0); 
// 	rtc_inst.alarmInterrupt(1, true);
// }


// // --- SET RTC ALARM RELATIVE
// void setRTCAlarm_Absolute(int hour, int minute, int second)
// {
// 	DateTime now = rtc_inst.now();

// 	// Display current time
// 	LOOM_DEBUG_Println("Current Time:");
// 	print_DateTime( now );

// 	// Calculate new time (checking to make sure alarm is not set in past)
// 	DateTime future = DateTime(now.year(), now.month(), now.day(), hour, minute, second);

// 	// If that time has already passed on current day, set for next day
// 	// Using Timespan of 24 hours to prevent errors
// 	if ( (future - now).totalseconds() < 0 ) {
// 		LOOM_DEBUG_Println("Time already passed, setting for next day");
// 		future = future + TimeSpan(1,0,0,0);
// 	} 

// 	LOOM_DEBUG_Println("Resetting Alarm 1 for:"); 
// 	print_DateTime(future);

// 	// Set alarm 1																	// not sure if this 0 is correct
// 	rtc_inst.setAlarm(ALM1_MATCH_HOURS, future.second(), future.minute(), future.hour(), 0); 	    								
// 	rtc_inst.alarmInterrupt(1, true);
// }



// // --- CLEAR RTC ALARMS ---
// //
// // Clear any pending alarms on the RTC
// //
// void clearRTCAlarms() {
// 	rtc_inst.armAlarm(1, false);
// 	rtc_inst.clearAlarm(1);
// 	rtc_inst.alarmInterrupt(1, false);
// 	rtc_inst.armAlarm(2, false);
// 	rtc_inst.clearAlarm(2);
// 	rtc_inst.alarmInterrupt(2, false);
// }



// void rtc_interrupt_reset()
// {
// 	// Clears any interrupts that may be pending on M0
// 	detachInterrupt(digitalPinToInterrupt(RTC_pin));
// 	delay(20);
// 	attachInterrupt(digitalPinToInterrupt(RTC_pin), wake_RTC_ISR, LOW);
// }
