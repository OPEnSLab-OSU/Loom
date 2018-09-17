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




// int x = 0;
// ================================================================ 
// ===                        MAIN LOOP                         ===
// ================================================================ 
void loop() 
{
	OSCBundle bndl, send_bndl;

	// receive_bundle(&bndl, WIFI);
	// if (bndl.size()) {
	// 	print_bundle(&bndl);
	// }
	// process_bundle(&bndl);

	measure_sensors();
	package_data(&send_bndl);
	LOOM_DEBUG_Println("Original Bundle:");
	print_bundle(&send_bndl);


	#if is_sd == 1
		log_bundle(&send_bndl, SDCARD, "ishield.csv");
	#endif

	// print_time();

	convert_bundle_structure(&send_bndl, &bndl, SINGLEMSG);
	LOOM_DEBUG_Println("Converted Bundle:");
	print_bundle(&bndl);


	// LOOM_DEBUG_Println("DONE");

	additional_loop_checks();


	delay(1000);

	// x++;

	// if (x > 1) {
		// while(1);
	// // }

	
	// LOOM_DEBUG_Println("*");

	// --- Common Example ---
	// OSCBundle bndl, send_bndl;

	// receive_bundle(&bndl, WIFI);
	// if (bndl.size()) {
	// 	print_bundle(&bndl);
	// }
	// process_bundle(&bndl);
	// // log_bundle(&bndl, PUSHINGBOX);

	// measure_sensors();
	// package_data(&send_bndl);
	// send_bundle(&send_bndl, WIFI);

	// additional_loop_checks();
	// // --- End Example ---

}


// ================================================================ 
// ===                 High-Level API Functions                 === 
// ================================================================ 

//
// void receive_bundle(OSCBundle *bndl, CommPlatform platform);
// void process_bundle(OSCBundle *bndl);
// void measure_sensors();
// void package_data(OSCBundle *send_bndl);
// void send_bundle(OSCBundle *send_bndl, CommPlatform platform, int port);
// void send_bundle(OSCBundle *send_bndl, CommPlatform platform);
// void log_bundle(OSCBundle *send_bndl, LogPlatform platform, char* file); //filename for SD files
// void log_bundle(OSCBundle *send_bndl, LogPlatform platform);
// bool bundle_empty(OSCBundle *bndl);
// void additional_loop_checks();
