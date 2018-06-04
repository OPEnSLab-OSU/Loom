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
	Loom_begin();		// LOOM_begin calls any relevant (based on config) LOOM device setup functions

	// Any custom setup code
}


// ================================================================ 
// ===                        MAIN LOOP                         ===
// ================================================================ 
void loop() 
{
	OSCBundle bndl, send_bndl; 

	// Receive bundles
	//  takes bundle to be filled and wireless platforms [WIFI_PLAT, LORA_PLAT, NRF_PLAT]
	receive_bundle(&bndl, WIFI_PLAT);

	// Process bundle (nothing will happen if bndl is empty), bundle is emptied after processing
	process_bundle(&bndl);

	// Update stored readings from sensors
	measure_sensors();

	// Populate bundle to send with sensor values
	package_data(&send_bndl);

	// Send the bundle
	//  takes bundle to be filled and wireless platforms [WIFI_PLAT, LORA_PLAT, NRF_PLAT]
	send_bundle(&send_bndl, WIFI_PLAT);
	
	// Loop checks and sleep between iterations if enabled
	additional_loop_checks();
	
} // End loop section


