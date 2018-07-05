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

	bndl.empty();
	// Receive bundles, takes bundle to be filled and wireless platforms [WIFI, LORA, NRF]
	// receive_bundle(&bndl, WIFI);

	// Process bundle (nothing will happen if bndl is empty), bundle is emptied after processing
	// process_bundle(&bndl);

// 	// Dummy Sapflow Data
	String parameters[7] = {"Test", "Battery", "Temp1", "Temp2", "Temp_Diff", "Temp_SHT31D", "Humidity_SHT31D"};
	String data[7] = {"abc", "4.27", "23.0", "25.2", "2.2", "30.0", "14"};

	convert_assoc_arrays_to_bundle(parameters, data, &bndl, "/some/packet/header", 7, SINGLEMSG);
	// convert_array_to_bundle(data, &bndl, "/some/packet/header", 7);
	print_bundle(&bndl);



	while(1);

	send_bundle(&bndl, PUSHINGBOX);

	delay(2000);

	send_bundle(&bndl, PUSHINGBOX);

	delay(2000);

	send_bundle(&bndl, PUSHINGBOX);

	while(1);



	delay(2000);

	// Loop checks and sleep between iterations if enabled
	additional_loop_checks();
	
} // End loop section








