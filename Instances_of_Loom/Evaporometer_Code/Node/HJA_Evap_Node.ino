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
	OSCBundle send_bndl;
	measure_sensors();
	package_data(&send_bndl);
	send_bundle(&send_bndl, LORA);
	delay(1000);
}


// ================================================================ 
// ===                        MAIN LOOP                         ===
// ================================================================ 
void loop() 
{
	OSCBundle send_bndl;

	digitalWrite(led, LOW);
	measure_sensors();
	package_data(&send_bndl);
	send_bundle(&send_bndl, LORA);
	digitalWrite(led, LOW);
	sleep_for(474, SECONDS, STANDBY);
	digitalWrite(led, LOW);

}
