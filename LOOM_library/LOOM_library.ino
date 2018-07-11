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

	// Receive bundles, takes bundle to be filled and wireless platforms [WIFI, LORA, NRF]
	// receive_bundle(&bndl, LORA);

	// if (!bundle_empty(&bndl)) {
	// 	print_bundle(&bndl);
	// }

	// Process bundle (nothing will happen if bndl is empty), bundle is emptied after processing
	// process_bundle(&bndl);


// 	// Dummy Sapflow Data
	// String parameters[7] = {"Test", "Battery", "Temp1", "Temp2", "Temp_Diff", "Temp_SHT31D", "Humidity_SHT31D"};
	// String data[7] = {"abc", "4.27", "23.0", "25.2", "2.2", "30.0", "1024"};
	// convert_assoc_arrays_to_bundle(parameters, data, &bndl, "/some/packet/header", 7, SINGLEMSG);
	// print_bundle(&bndl);
	LOOM_DEBUG_Println("Starting main loop");
	// LOOM_DEBUG_Println("Delaying to allow Fona to connect");
	// delay(15000);
	// LOOM_DEBUG_Println("Done delaying");

	send_bundle(&bndl, PUSHINGBOX);
	while(1);

	// fona_test_loop(); // will enter the Fona test loop, which doesn't return until 'Q' is entered into the Fona REPL
	// while(1);



	// measure_sensors();
	// send_bundle(&send_bndl, WIFI);

	// Loop checks and sleep between iterations if enabled
	additional_loop_checks();
	
} // End loop section








