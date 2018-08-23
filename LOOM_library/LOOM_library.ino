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
	OSCBundle bndl; 
	// OSCBundle send_bndl;

	receive_bundle(&bndl, LORA);
	print_bundle(&bndl);

	log_bundle(&bndl, PUSHINGBOX);

	additional_loop_checks();

}
