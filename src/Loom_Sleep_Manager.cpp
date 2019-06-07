
#include "Loom_Sleep_Manager.h"
#include "RTC/Loom_RTC.h"

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
		const char*		module_name, 
		bool			use_LED, 
		bool 			delay_on_wake, 
		SleepMode		sleep_mode,
		byte			power_off_pin
	) : LoomModule( module_name )
{
	this->module_type = ModuleType::Sleep_Manager;

	this->use_LED		= use_LED;
	this->delay_on_wake	= delay_on_wake;
	this->sleep_mode	= SleepMode::STANDBY;

	this->power_off_pin	= power_off_pin;
	pinMode(power_off_pin, OUTPUT);
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_Sleep_Manager::Loom_Sleep_Manager(JsonVariant p)
	: Loom_Sleep_Manager(p[0], p[1], p[2], (SleepMode)(int)p[3], p[4])
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_Sleep_Manager::~Loom_Sleep_Manager()
{

}

/////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::link_interrupt_manager(Loom_Interrupt_Manager* IM)
{
	interrupt_manager = IM;
}

/////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::print_config()
{
	LoomModule::print_config();
	LPrintln('\t', "Sleep Mode          : ", enum_sleep_mode_string(sleep_mode) );
	LPrintln('\t', "Use LED             : ", (use_LED) ? "Enabled" : "Disabled" );
	LPrintln('\t', "Delay on Wake       : ", (delay_on_wake) ? "Enabled" : "Disabled" );
}

/////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::print_state()
{
	LoomModule::print_state();
}

/////////////////////////////////////////////////////////////////////

void Loom_Sleep_Manager::powerDown()
{
	// Call manager to coordinate modules saving to flash
	// and running any individual powerDown methods

	LPrintln("Powering Off");
	digitalWrite(power_off_pin, HIGH);
	delay(50); // Sometimes the board takes several milliseconds to fully power off
	LPrintln("This should not be printed");
}














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
// 			// LPrintln("Will try to sleep until : ");
// 			// RTC_Inst->print_DateTime(future_time);


// 			// // Cannot wake at time in the past
// 			// DateTime now = RTC_Inst->now();
// 			// if ( (future_time - now).totalseconds() < 0 ) {
// 			// 	print_module_label();
// 			// 	LPrintln("Wont wake from alarm in the past, increasing time to following day");
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
// 			// 	LPrintln("Will instead try to sleep until : ");
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

		LowPower.standby();
		// switch(sleep_mode) {
			// case SleepMode::STANDBY : LowPower.standby(); 
				// break;
			// case SleepMode::IDLE 	: LowPower.idle(IDLE_2); 
				// break;

			// implement
			// case SleepMode::SLEEPYDOG 	: return false;   

			// implement
			// case SleepMode::OPENS_LOWPOWER 	: return false;
		// }

		// This is where programs waits until waking

		post_sleep();
}

/////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::pre_sleep()
{
	LPrintln("\nEntering STANDBY");
	delay(50);
	Serial.end();
	USBDevice.detach();



	// Don't know why this has to happen twice but it does
	// attachInterrupt(digitalPinToInterrupt(WAKE_PIN), wake_ISR, LOW);
	// attachInterrupt(digitalPinToInterrupt(WAKE_PIN), wake_ISR, LOW);

	digitalWrite(LED_BUILTIN, LOW);
}

/////////////////////////////////////////////////////////////////////
void Loom_Sleep_Manager::post_sleep()
{

	// Prevent double trigger of alarm interrupt
	if (interrupt_manager) {
		interrupt_manager->get_RTC_module()->clear_alarms();
	}

	// if (use_LED) {
		digitalWrite(LED_BUILTIN, HIGH);
	// }

	#if LOOM_DEBUG == 1
		USBDevice.attach();
		Serial.begin(115200);
		if (delay_on_wake) {
			// Give user 5s to reopen Serial monitor!
			// Note that the serial may still take a few seconds 
			// to fully setup after the LED turns on
			delay(5000); 
		}
		LPrintln("WAKE");
	#endif

	// Set wake time

}
