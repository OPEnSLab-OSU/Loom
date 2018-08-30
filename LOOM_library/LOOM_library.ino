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

	receive_bundle(&bndl, LORA);
	if (bndl.size()) {
		print_bundle(&bndl);
	}
	process_bundle(&bndl);
	log_bundle(&bndl, PUSHINGBOX);

	measure_sensors();
	package_data(&send_bndl);
	send_bundle(&send_bndl, LORA);

	additional_loop_checks();

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
