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

	digitalWrite(led, HIGH);

	// Any custom setup code

}

int x = 1;
// ================================================================ 
// ===                        MAIN LOOP                         ===
// ================================================================ 
void loop() 
{

	flash_led(x++, 400, 100);
	x %= 10;
	digitalWrite(LED_BUILTIN, HIGH);
	delay(1000);
	flash_led(8, 80, 30);


	sleep_for(5, SECONDS, STANDBY);

	sleep_for(5, SECONDS, SLEEPYDOG);



	// OSCBundle bndl, send_bndl;

	// receive_bundle(&bndl, WIFI);
	// process_bundle(&bndl);
	// measure_sensors();
	// package_data(&send_bndl);
	// send_bundle(&send_bndl, WIFI);
	// additional_loop_checks();

}