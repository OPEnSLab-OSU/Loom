// ================================================================
// ===              INCLUDE CONFIGURATION FILE                  ===
// ===    INCLUDE DECLARATIONS, STRUCTS, AND FUNCTIONS FROM     ===
// ===            OTHER FILES AS SET IN CONFIG.H                ===
// ================================================================

// Config has to be first has it hold all user specified options
#include "config.h"

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

  run_sapflowmeter(&bndl);
//  #if is_hub == 1
//  
//  	// Receive bundles, takes bundle to be filled and wireless platforms [WIFI, LORA, NRF]
//  	  receive_bundle(&bndl, LORA);
//  
//    if (!bundle_empty(&bndl)) {
//      print_bundle(&bndl);
//      send_bundle(&bndl, PUSHINGBOX);
//  
//    }
//  #endif // of is_hub
//
//  #if is_node == 1
//    measure_sensors();
//    package_data(&send_bndl);
//    print_bundle(&send_bndl);
//    send_bundle(&send_bndl, LORA);
//    sd_save_bundle("Log0711.csv", &send_bndl, 0, 3);
//    //  read_all_from_file("newlog");
//    
//    //  heat(2500);
//   delay(60000); //send data per 1 min
//  
//  #endif // of is_node

	// Loop checks and sleep between iterations if enabled
	additional_loop_checks();
	
} // End loop section








