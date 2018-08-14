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
	OSCBundle bndl, send_bndl; 
	
	// receive_bundle(&bndl, LORA);
	// process_bundle(&bndl);
	
	measure_sensors();

	package_data(&send_bndl);

//	send_bndl.add("/test").add((int32_t)42);

	print_bundle(&send_bndl);
	
	// String keys[8], vals[8];
	// convert_bundle_to_arrays_assoc(&send_bndl, keys, vals, 8);
	// print_array(keys, 8, 2);
	// print_array(vals, 8, 2);
	// LOOM_DEBUG_Println();

	send_bundle(&send_bndl, LORA);
	delay(5000);

	additional_loop_checks();

	
} // End loop section









