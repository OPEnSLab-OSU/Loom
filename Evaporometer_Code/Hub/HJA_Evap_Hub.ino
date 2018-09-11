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
	bndl.empty(); send_bndl.empty();

	receive_bundle(&bndl, LORA);
	if (bndl.size()) {

		if ( ( !strstr(get_address_string(bndl.getOSCMessage(0)).c_str(), "Sap") ) && (!strstr(get_address_string(bndl.getOSCMessage(0)).c_str(), "Ping") )) { 
			print_bundle(&bndl);
		}

		if ( strstr(get_address_string(bndl.getOSCMessage(0)).c_str(), "Ping") ) {
			process_bundle(&bndl);		
		} else {
			log_bundle(&bndl, PUSHINGBOX);
		}
	}

	delay(50);
	// additional_loop_checks();

}
