// ================================================================
// ===              INCLUDE CONFIGURATION FILE                  ===
// ===    INCLUDE DECLARATIONS, STRUCTS, AND FUNCTIONS FROM     ===
// ===            OTHER FILES AS SET IN CONFIG.H                ===
// ================================================================

// Config has to be first has it hold all user specified options
#include "config.h"


//#define hub_node_type 1
// Preamble includes any relevant subroutine files based 
// on options specified in the above config
#include "loom_preamble.h"

void newWakeUp();
volatile byte ledOnOff = LOW;
volatile bool write = 0;


// ================================================================ 
// ===                           SETUP                          ===
// ================================================================ 
void setup() 
{
	// LOOM_begin calls any relevant (based on config) LOOM device setup functions
	Loom_begin();	

	digitalWrite(led, LOW);

	// Any custom setup code
	LOOM_DEBUG_Println2("Time: ", __TIME__);
	LOOM_DEBUG_Println2("Date: ", __DATE__);

	// LOOM_DEBUG_Println("\nReady");

	// sleep_for(10, SECONDS, STANDBY);

	pinMode(9, INPUT_PULLUP);

	// pciSetup(11); 

    attachInterrupt(digitalPinToInterrupt(9), newWakeUp, LOW);

}

// ================================================================ 
// ===                        MAIN LOOP                         ===
// ================================================================ 
void loop() 
{

	digitalWrite(led, ledOnOff);

	if (write) {
		LOOM_DEBUG_Println("RTC interrupt happened");
		write = 0;
		delay(75);	
	}
	
	// OSCBundle bndl, send_bndl;

	// receive_bundle(&bndl, WIFI);
	// process_bundle(&bndl);
	// measure_sensors();
	// package_data(&send_bndl);
	// send_bundle(&send_bndl, WIFI);
	// additional_loop_checks();
	// while(1);


	// digitalWrite(led, HIGH);
	// delay(5000);
	// LOOM_DEBUG_Println("\nIn Main Loop");
	// delay(5000);
	// LOOM_DEBUG_Println("Still in main loop");
	// delay(1000);
	// LOOM_DEBUG_Println("About to sleep");
	// digitalWrite(led, LOW);

	// sleep_for(6, SECONDS, SLEEPYDOG);
	// sleep_for(5000, MILLIS, STANDBY);

	// LOOM_DEBUG_Println("\n");

	// sleep_until_interrupt(0, IDLE);

	
} // End loop section

// void pciSetup(byte pin)
// {
// 	*digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
// 	PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
// 	PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
// }


void newWakeUp() {
	ledOnOff = !ledOnOff;
	write = true;
	// pciSetup(9);
	// PCICR = 0x00;         // Disable PCINT interrupt
	clearAlarms(); // Clear RTC Alarm

	// rtc_inst.setAlarm(ALM1_MATCH_HOURS, MIN, HR, 0);   //set your wake-up time here
	// rtc_inst.alarmInterrupt(1, true);


    DateTime future( rtc_inst.now() + TimeSpan(0,0,0,7) );
	rtc_inst.setAlarm(ALM1_MATCH_HOURS, future.second(), future.minute(), future.hour(), 0); 
	// rtc_inst.armAlarm(1, true);
	rtc_inst.alarmInterrupt(1, true);


}





