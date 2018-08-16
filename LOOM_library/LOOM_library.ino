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


// ================================================================ 
// ===                           SETUP                          ===
// ================================================================ 
void setup() 
{
	// LOOM_begin calls any relevant (based on config) LOOM device setup functions
	Loom_begin();	

	// Any custom setup code
}

// ================================================================ 
// ===                        MAIN LOOP                         ===
// ================================================================ 
void loop() 
{
	// OSCBundle bndl, send_bndl;

	// receive_bundle(&bndl, WIFI);
	// process_bundle(&bndl);
	// measure_sensors();
	// package_data(&send_bndl);
	// send_bundle(&send_bndl, WIFI);
	// additional_loop_checks();
	digitalWrite(led, HIGH);
	LOOM_DEBUG_Println("\nIn Main Loop");
	delay(5000);
	LOOM_DEBUG_Println("Still in main loop");
	delay(1000);
	LOOM_DEBUG_Println("About to sleep");
	digitalWrite(led, LOW);
	sleep_for(6, SEC, SLEEPYDOG);


	for (int i = 0; i < 3; i++) {
		digitalWrite(led, HIGH);
		delay(500);
		digitalWrite(led, LOW);
		delay(300);
	}
	
} // End loop section









