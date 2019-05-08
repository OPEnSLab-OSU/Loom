
#include "Loom_Sleep_Manager.h"
#include "RTC/Loom_RTC.h"

#include "Loom_Interrupt_Manager.h"

#include <Adafruit_SleepyDog.h>
#include <LowPower.h>



/////////////////////////////////////////////////////////////////////
const char* Loom_Sleep_Manager::enum_sleep_mode_string(SleepMode m)
{
	switch(m) {
		case SleepMode::IDLE			: return "Idle";
		case SleepMode::STANDBY			: return "Standby";
		case SleepMode::SLEEPYDOG		: return "SleepyDog";
		case SleepMode::OPENS_LOWPOWER	: return "OPEnS_Lowpower";
		default : return "";
	}
}

/////////////////////////////////////////////////////////////////////
Loom_Sleep_Manager::Loom_Sleep_Manager( 
		const char*	module_name, 
		bool		use_LED, 
		bool 		delay_on_wake, 
		SleepMode	sleep_mode 
	) : LoomModule( module_name )
{
	this->module_type = ModuleType::Sleep_Manager;

	this->use_LED		= use_LED;
	this->delay_on_wake	= delay_on_wake;
	this->sleep_mode	= SleepMode::STANDBY;
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_Sleep_Manager::Loom_Sleep_Manager(JsonVariant p)
	: Loom_Sleep_Manager(p[0], p[2], p[3], (SleepMode)(int)p[4])
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_Sleep_Manager::~Loom_Sleep_Manager()
{

}

/////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::print_config()
{
	LoomModule::print_config();
	Println3('\t', "Sleep Mode          : ", enum_sleep_mode_string(sleep_mode) );
	Println3('\t', "Use LED             : ", (use_LED) ? "Enabled" : "Disabled" );
}

/////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::print_state()
{
	LoomModule::print_state();
}

/////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::link_device_manager(LoomManager* LM)
{
	LoomModule::link_device_manager(LM);
}

// /////////////////////////////////////////////////////////////////////
// void Loom_Sleep_Manager::set_Interrupt_Manager(Loom_Interrupt_Manager* IM)
// {
// 	this->IM = IM;
// }

// /////////////////////////////////////////////////////////////////////
// Loom_Interrupt_Manager* Loom_Sleep_Manager::get_Interrupt_Manager()
// {
// 	return IM;
// }

/////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::set_sleep_mode(SleepMode mode)
{
	sleep_mode = mode;
}

/////////////////////////////////////////////////////////////////////
SleepMode Loom_Sleep_Manager::get_sleep_mode()
{
	return sleep_mode;
}



// /////////////////////////////////////////////////////////////////////
// bool Loom_Sleep_Manager::sleep_until(DateTime future_time)
// {
// 	switch(sleep_mode) {
		
// 		// Both follow same process, intentional fallthrough
// 		case SleepMode::STANDBY :
// 		case SleepMode::IDLE :
// 		{
// 			// Don't sleep if no RTC to wake up device
// 			// if (RTC_Inst == nullptr) {
// 			// 	return false;
// 			// }

// 			// print_module_label();
// 			// Println("Will try to sleep until : ");
// 			// RTC_Inst->print_DateTime(future_time);


// 			// // Cannot wake at time in the past
// 			// DateTime now = RTC_Inst->now();
// 			// if ( (future_time - now).totalseconds() < 0 ) {
// 			// 	print_module_label();
// 			// 	Println("Wont wake from alarm in the past, increasing time to following day");
// 			// 	// future_time = future + TimeSpan(1,0,0,0);    // might not work if DateTime is several days in past
				
// 			// 	// Adjust future_time to be following day at same time intended
// 			// 	future_time = DateTime(	now.year(), 
// 			// 							now.month(), 
// 			// 							now.day(), 
// 			// 							future_time.hour(), 
// 			// 							future_time.minute(), 
// 			// 							future_time.second() )
// 			// 					+ TimeSpan(1,0,0,0);

// 			// 	print_module_label();
// 			// 	Println("Will instead try to sleep until : ");
// 			// 	RTC_Inst->print_DateTime(future_time);

// 			// }

// 			// // Tell RTC_Inst to set RTC time at future_time
// 			// // Then call sleep_until_interrupt on pin, because that is what it is
// 			// RTC_Inst->set_alarm(future_time);
			

// 			// if ( IM ){
// 			// 	IM->RTC_alarm_exact(future_time);
// 			// } 

			

// 			// sleep();


// 			// digitalWrite(LED_BUILTIN, HIGH);

// 			return true;
// 		}
// 		case SleepMode::SLEEPYDOG : 
// 			return false; // Can't sleep until a given time unless using RTC, in which case, use Standby or Idle instead
		
// 		case SleepMode::OPENS_LOWPOWER : 
// 			return false; 
// 	}
// }


/////////////////////////////////////////////////////////////////////
bool Loom_Sleep_Manager::sleep()
{
		pre_sleep();

		switch(sleep_mode) {
			case SleepMode::STANDBY : LowPower.standby(); 
			case SleepMode::IDLE 	: LowPower.idle(IDLE_2); 

			// implement
			// case SleepMode::SLEEPYDOG 	: return false;   

			// implement
			// case SleepMode::OPENS_LOWPOWER 	: return false;


		}

		// This is where programs waits until waking

		post_sleep();
}

/////////////////////////////////////////////////////////////////////
#define MAX_WATCHDOG_SLEEP 16 // seconds

bool Loom_Sleep_Manager::sleepy_dog_sleep(TimeSpan duration)
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

/////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::pre_sleep()
{
	Println("Entering Sleep");
	#if LOOM_DEBUG == 1
		Serial.end();
		USBDevice.detach(); 
	#endif

	// Interrupt/Alarm reset 

	// if (IM) {
	// 	IM->interrupt_reset(RTC_Inst->get_interrupt_pin());
	// }
	// RTC_Inst->rtc_interrupt_reset(); //clear interrupt registers, attach interrupts


	delay(50);
	digitalWrite(LED_BUILTIN, LOW);
}

/////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::post_sleep()
{

// Standy by might get its own pre and post sleeps which in turn calls

	// if (IM) {
	// 	IM->interrupt_reset(RTC_Inst->get_interrupt_pin());
	// }
	// // rtc_interrupt_reset(); //clear interrupt registers, attach interrupts
	// // RTC_Inst->clear_alarms(); //prevent double trigger of alarm interrupt


	// // Not sure why these have to be repeated but it seems to make a difference
	// if (IM) {
	// 	IM->interrupt_reset(RTC_Inst->get_interrupt_pin());
	// }
	// rtc_interrupt_reset();
	// RTC_Inst->clear_alarms();


	// Update last_wake_time
	// last_wake_time = (RTC_Inst) ? RTC_Inst->now() : DateTime(0);

	// Reconnect USB
	#if LOOM_DEBUG == 1
		USBDevice.attach();
		Serial.begin(SERIAL_BAUD);
	#endif

	// Turn on LED
	if (use_LED) {
		digitalWrite(LED_BUILTIN, HIGH);
	}

	// Give user time to open Serial (only when debugging)
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

		// if (RTC_Inst != nullptr) {
		// 	print_module_label();
		// 	Print("Wake Time : ");
		// 	RTC_Inst->print_time();
		// }
		delay(50);  // delay so serial stuff has time to print out all the way
	#endif

	// Turn of LED
	digitalWrite(LED_BUILTIN, LOW);

}
