// ================================================================
// ===              INCLUDE CONFIGURATION FILE                  ===
// ===    INCLUDE DECLARATIONS, STRUCTS, AND FUNCTIONS FROM     ===
// ===            OTHER FILES AS SET IN CONFIG.H                ===
// ================================================================

// Config has to be first has it hold all user specified options
#include "config.h"

// Preamble includes any relevant subroutine files based on options
// specified in the above config
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

	// Receive bundles, takes bundle to be filled and wireless platforms [WIFI, LORA, NRF]
	 receive_bundle(&bndl, WIFI);


	// Single to multi test
	if (get_bundle_bytes(&bndl)) {
		LOOM_DEBUG_Println("Got a bundle");

//		LOOM_DEBUG_Println("\nOLD");
//		print_bundle(&bndl);

//		convert_OSC_singleMsg_to_multiMsg(&bndl, &send_bndl);
//		convert_OSC_singleMsg_to_multiMsg_in_place(&bndl);
		deep_copy_bundle(&bndl, &send_bndl);

		LOOM_DEBUG_Println("\nOLD");
		print_bundle(&bndl);
		LOOM_DEBUG_Println("\nNEW");
		print_bundle(&send_bndl);
		
		while(1);
	}





	// Process bundle (nothing will happen if bndl is empty), bundle is emptied after processing
//	process_bundle(&bndl);

	// Update stored readings from sensors
//	measure_sensors();

	// Populate bundle to send with sensor values
//	package_data(&send_bndl);

	// Send the bundle, takes bundle to be filled and platform to send it over [WIFI, LORA, NRF, SDCARD, PUSHINGBOX]
//	send_bundle(&send_bndl, PUSHINGBOX);
//
//	if (get_bundle_bytes(&bndl)){
//		send_bundle(&bndl, SDCARD, "test2.txt");
//		read_all_from_file("test2.txt");
////		while(1);
//	}
	
	// Loop checks and sleep between iterations if enabled
	additional_loop_checks();
	
} // End loop section










