
#include "Loom_Sleep_Manager.h"
#include "Loom_RTC.h"

#include <Adafruit_SleepyDog.h>
#include <LowPower.h>




const char* Loom_Sleep_Manager::enum_sleep_mode_string(SleepMode m)
{
	switch(m) {
		case IDLE_SLEEP : return "IdleSleep";
		case STANDBY    : return "Standby";
		case SLEEPYDOG  : return "SleepyDog";
		default         : return "";
	}
}




Loom_Sleep_Manager::Loom_Sleep_Manager( char* module_name, LoomRTC* RTC_Inst, bool use_LED, bool delay_on_wake ) : LoomModule( module_name )
{
	this->use_LED 		= use_LED;
	this->delay_on_wake	= delay_on_wake;

	this->RTC_Inst = RTC_Inst;

	// Get current time
	if (this->RTC_Inst != NULL) {
		last_wake_time = this->RTC_Inst->now();
		// this->sleep_mode = STANDBY;
	} else {
		// this->sleep_mode = SLEEPYDOG;
	}
}

// Loom_Sleep_Manager( char* module_name, LoomManager* LD );



// --- DESTRUCTOR ---
Loom_Sleep_Manager::~Loom_Sleep_Manager()
{

}

void Loom_Sleep_Manager::print_config()
{
	LoomModule::print_config();
	// Println3('\t', "Sleep Mode          : ", enum_sleep_mode_string(sleep_mode) );
	Println3('\t', "Use LED             : ", (use_LED) ? "Enabled" : "Disabled" );
}


void Loom_Sleep_Manager::print_state()
{
	LoomModule::print_state();
	if (RTC_Inst != NULL) {
		Print2('\t', "Last Wake Time      : " );
		RTC_Inst->print_time();
	}
}



void Loom_Sleep_Manager::set_RTC_module(LoomRTC* RTC_Inst)
{
	this->RTC_Inst = RTC_Inst;
}

LoomRTC* Loom_Sleep_Manager::get_RTC_module()
{
	return RTC_Inst;
}




// void Loom_Sleep_Manager::set_sleep_mode(SleepMode mode)
// {
// 	sleep_mode = mode;
// }

// SleepMode Loom_Sleep_Manager::get_sleep_mode()
// {
// 	return sleep_mode;
// }





bool Loom_Sleep_Manager::sleep_for_time(TimeSpan duration)
{
	if (RTC_Inst != NULL) {
		// Calculate time to sleep until
		return sleep_until_time(RTC_Inst->now() + duration);	
	} else {
		return false;
	}

	// return (RTC_Inst != NULL) ? sleep_until_time(RTC_Inst->now() + duration_) : false;
}

bool Loom_Sleep_Manager::sleep_for_time(uint days, uint hours, uint minutes, uint seconds)
{
	return sleep_for_time( TimeSpan(days, hours, minutes, seconds) );
}


bool Loom_Sleep_Manager::sleep_for_time_from_wake(TimeSpan duration)
{
	if (RTC_Inst != NULL) {
		// Calculate time to sleep until
		return sleep_until_time(last_wake_time + duration);	
	} else {
		return false;
	}

	return true;
}

bool Loom_Sleep_Manager::sleep_for_time_from_wake(uint days, uint hours, uint minutes, uint seconds)
{
	return sleep_for_time_from_wake( TimeSpan(days, hours, minutes, seconds) );

}


bool Loom_Sleep_Manager::sleep_until_time(DateTime future_time)
{
	// switch(sleep_mode) {
		// case STANDBY :
			// Need RTC to wake from standby at a given time 
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

bool Loom_Sleep_Manager::sleep_until_time(uint hour, uint minute, uint second)
{
	if (RTC_Inst == NULL) {
		return false;
	}

	// Call sleep_until_time(DateTime future_time) with that time today
	// That function will adjust to following day if necessary
	DateTime now = RTC_Inst->now();
	return sleep_until_time( DateTime(now.year(), now.month(), now.day(), hour, minute, second) ); 
}


bool Loom_Sleep_Manager::sleep_until_interrupt_on(byte pin)
{
		pre_sleep();

		LowPower.standby(); 
		// This is where programs waits until waking

		post_sleep();
}





void Loom_Sleep_Manager::sleepy_dog_sleep()
{

}



void Loom_Sleep_Manager::pre_sleep()
{
	Println("Entering STANDBY");
	#if LOOM_DEBUG == 1
		Serial.end();
		USBDevice.detach(); 
	#endif
	// RTC_Inst->rtc_interrupt_reset(); //clear interrupt registers, attach interrupts
	delay(50);
	digitalWrite(LED_BUILTIN, LOW);
}

void Loom_Sleep_Manager::post_sleep()
{
	// update last_wake_time

	if (use_LED) {
		digitalWrite(LED_BUILTIN, HIGH);
	}

	#if LOOM_DEBUG == 1
		if (delay_on_wake) {
			delay(5000); // give user 5s to close and reopen serial monitor!

			if (use_LED) {
				digitalWrite(LED_BUILTIN, LOW);
				delay(100);
				digitalWrite(LED_BUILTIN, HIGH);
			}
		}
	#endif

	// rtc_interrupt_reset(); //clear interrupt registers, attach interrupts
	// clearRTCAlarms(); //prevent double trigger of alarm interrupt

	// Not sure why these have to be repeated but it seems to make a difference
	// rtc_interrupt_reset();
	// clearRTCAlarms();

	#if LOOM_DEBUG == 1
		USBDevice.attach();
		Serial.begin(SERIAL_BAUD);
		Println("WAKE");

		if (RTC_Inst != NULL) {
			print_module_label();
			Print("Wake Time : ");
			RTC_Inst->print_time();
		}
		delay(50);  // delay so serial stuff has time to print out all the way
	#endif
}
