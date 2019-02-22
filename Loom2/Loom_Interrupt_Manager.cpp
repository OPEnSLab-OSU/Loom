
#include "Loom_Interrupt_Manager.h"

// #include "Loom_Sleep_Manager.h"
// #include "Loom_RTC.h"

// #include <Adafruit_SleepyDog.h>
// #include <LowPower.h>







Loom_Interrupt_Manager::Loom_Interrupt_Manager( char* module_name) 
	: LoomModule( module_name )
{

}


// --- DESTRUCTOR ---
Loom_Interrupt_Manager::~Loom_Interrupt_Manager()
{

}

void Loom_Interrupt_Manager::print_config()
{
	LoomModule::print_config();

}


void Loom_Interrupt_Manager::print_state()
{
	LoomModule::print_state();

	// print out registered interrupts

}



void Loom_Interrupt_Manager::set_RTC_module(LoomRTC* RTC_Inst)
{
	this->RTC_Inst = RTC_Inst;
}

LoomRTC* Loom_Interrupt_Manager::get_RTC_module()
{
	return RTC_Inst;
}




// void Loom_Interrupt_Manager::set_sleep_mode(SleepMode mode)
// {
// 	sleep_mode = mode;
// }

// SleepMode Loom_Interrupt_Manager::get_sleep_mode()
// {
// 	return sleep_mode;
// }





bool Loom_Interrupt_Manager::sleep_for_time(TimeSpan duration, SleepMode mode)
{
	switch(mode) {

		case STANDBY : 
			// Try sleeping with Standby unless no RTC object
			// Calculate time to sleep until
			if (RTC_Inst) {
				return sleep_until_time(RTC_Inst->now() + duration);
			} 

			// Intentional fallthrough: if no RTC found, revert to SLEEPYDOG

		case SLEEPYDOG : 
			// Sleep 'hack' using repeated calls to Watchdog.sleep()
			return sleepy_dog_sleep(duration);
	}
}

bool Loom_Interrupt_Manager::sleep_for_time(uint days, uint hours, uint minutes, uint seconds, SleepMode mode)
{
	return sleep_for_time( TimeSpan(days, hours, minutes, seconds) );
}

bool Loom_Interrupt_Manager::sleep_for_time_from_wake(TimeSpan duration)
{
	return (RTC_Inst) ? sleep_until_time(last_wake_time + duration) : false;
}

bool Loom_Interrupt_Manager::sleep_for_time_from_wake(uint days, uint hours, uint minutes, uint seconds)
{
	return sleep_for_time_from_wake( TimeSpan(days, hours, minutes, seconds) );

}


bool Loom_Interrupt_Manager::sleep_until_time(DateTime future_time)
{
	// switch(sleep_mode) {
		// case STANDBY :
			
			// Don't sleep if no RTC to wake up device
			if (RTC_Inst == NULL) {
				return false;
			}

			print_module_label();
			Println("Will try to sleep until : ");
			RTC_Inst->print_DateTime(future_time);


			// Cannot wake at time in the past
			DateTime now = RTC_Inst->now();
			if ( (future_time - now).totalseconds() < 0 ) {
				print_module_label();
				Println("Wont wake from alarm in the past, increasing time to following day");
				// future_time = future + TimeSpan(1,0,0,0);    // might not work if DateTime is several days in past
				
				// Adjust future_time to be following day at same time intended
				future_time = DateTime(	now.year(), 
										now.month(), 
										now.day(), 
										future_time.hour(), 
										future_time.minute(), 
										future_time.second() )
								+ TimeSpan(1,0,0,0);

				print_module_label();
				Println("Will instead try to sleep until : ");
				RTC_Inst->print_DateTime(future_time);

			}

			// RTC_Inst->setRTCAlarm_Absolute(hour, min, sec);

			// Tell RTC_Inst to set RTC time at future_time
			// Then call sleep_until interrupt on pin, because that is what it is

			// sleep_until_interrupt_on(  RTC pin );


			// break;
	// }


}

bool Loom_Interrupt_Manager::sleep_until_time(uint hour, uint minute, uint second)
{
	// Don't sleep if no RTC to wake up device
	if (RTC_Inst == NULL) {
		return false;
	}

	// Call sleep_until_time(DateTime future_time) with that time today
	// That function will adjust to following day if necessary
	DateTime now = RTC_Inst->now();
	return sleep_until_time( DateTime(now.year(), now.month(), now.day(), hour, minute, second) ); 
}


bool Loom_Interrupt_Manager::sleep_until_interrupt_on(byte pin)
{
		pre_sleep();

		LowPower.standby(); 
		// This is where programs waits until waking

		post_sleep();
}




#define MAX_WATCHDOG_SLEEP 8 // seconds

bool Loom_Interrupt_Manager::sleepy_dog_sleep(TimeSpan duration)
{
	uint32_t totalseconds = duration.totalseconds();

	int iterations = totalseconds / MAX_WATCHDOG_SLEEP;
	int remainder  = totalseconds % MAX_WATCHDOG_SLEEP;
	int total = 0; // counter (mostly for display purposes currently)

	Println3("Will sleep for a total of: ", totalseconds, " seconds");
	Print5("Using ", iterations, " blocks of ", MAX_WATCHDOG_SLEEP, " seconds");
	Println3(" and ", remainder, " seconds");


	pre_sleep();


	// Println3("Going to sleep in ", MAX_WATCHDOG_SLEEP, " second blocks");
	for (int i = 0; i < iterations; i++) {
		int sleepMS = Watchdog.sleep(MAX_WATCHDOG_SLEEP * 1000);
		// Println3("Just slept for: ", sleepMS, " milliseconds");
		total += sleepMS;
		// Println3("Now have slept a total of: ", total, " milliseconds");

		// Test print blink
		// digitalWrite(LED_BUILTIN, HIGH);  
		// delay(100);                       
		// digitalWrite(LED_BUILTIN, LOW);   
	}

	if (remainder > 0) {
		// Println3("Sleeping the remaining ", remainder*1000, " milliseconds");
		int sleepMS = Watchdog.sleep(remainder*1000); // remained is in seconds
		total += sleepMS;
	}

	post_sleep();

	Println3("Done sleeping a total of: ", total, " milliseconds");

}





void Loom_Interrupt_Manager::pre_sleep()
{
	Println("Entering Sleep");
	#if LOOM_DEBUG == 1
		Serial.end();
		USBDevice.detach(); 
	#endif


	// RTC_Inst->rtc_interrupt_reset(); //clear interrupt registers, attach interrupts
	

	delay(50);
	digitalWrite(LED_BUILTIN, LOW);
}


void Loom_Interrupt_Manager::post_sleep()
{

// Standy by might get its own pre and post sleeps which in turn calls

	// rtc_interrupt_reset(); //clear interrupt registers, attach interrupts
	// clearRTCAlarms(); //prevent double trigger of alarm interrupt

	// Not sure why these have to be repeated but it seems to make a difference
	// rtc_interrupt_reset();
	// clearRTCAlarms();


	// Update last_wake_time
	last_wake_time = (RTC_Inst) ? RTC_Inst->now() : DateTime(0);

	// Reconnect USB
	#if LOOM_DEBUG == 1
		USBDevice.attach();
		Serial.begin(SERIAL_BAUD);
	#endif

	// Turn on LED
	if (use_LED) {
		digitalWrite(LED_BUILTIN, HIGH);
	}

	#if LOOM_DEBUG == 1
		if (delay_on_wake) {

			if (use_LED) {
				for (int i = 0; i < 4; i++) {
					digitalWrite(LED_BUILTIN, LOW);
					delay(100);
					digitalWrite(LED_BUILTIN, HIGH);
				}
			}

			delay(5000); // give user 5s to close and reopen serial monitor!

		}
	#endif

	// Print to Serial now that LED/delay has given user time to open Serial Monitor
	#if LOOM_DEBUG == 1
		Println("WAKE");

		if (RTC_Inst != NULL) {
			print_module_label();
			Print("Wake Time : ");
			RTC_Inst->print_time();
		}
		delay(50);  // delay so serial stuff has time to print out all the way
	#endif

	// Turn of LED
	digitalWrite(LED_BUILTIN, LOW);

}



