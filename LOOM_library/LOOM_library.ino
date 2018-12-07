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
	
	// --- Basic Example

	// NRF NODE

	receive_bundle(&bndl, NRF);		// Read sensors, store data in sensor state struct
	if (bndl.size()) {
		LOOM_DEBUG_Println("\n\n\nRecieved bundle from hub");
		print_bundle(&bndl);


		LOOM_DEBUG_Println("End of bundle from hub\n\n\n");
	}



	measure_sensors();					// Read sensors, store data in sensor state struct
	package_data(&bndl);			// Copy sensor data from state to provided bundle
	
	print_bundle(&bndl);

	// send_bundle(&bndl, NRF);
	send_bundle(&bndl, NRF, 00);

	delay(1000);

	additional_loop_checks();			// Miscellaneous checks

	// --- End Example ---




	// NRF HUB

	// if ( (millis() - lastMillis) > 5000 ) {


	// 	LOOM_DEBUG_Println("\n\n\nSending bundle to node");
		
	// 	char address_string[80]; 
	// 	sprintf(address_string, "%s%s", configuration.packet_header_string, "/number");
	// 	bndl.add(address_string).add((int32_t)i++); 

	// 	print_bundle(&bndl);

	// 	send_bundle(&bndl, NRF);
	// 	bndl.empty();

	// 	lastMillis = millis();

	// 	LOOM_DEBUG_Println("Done Sending bundle to node\n\n\n");

	// }


	// receive_bundle(&bndl, NRF);

	// print_bundle(&bndl); 

	// additional_loop_checks();
	// // --- End Example ---







	// // --- OLED Example ---

	// measure_sensors();					// Read sensors, store data in sensor state struct
	// package_data(&bndl);			// Copy sensor data from state to provided bundle
	
	// print_bundle(&bndl);
	// log_bundle(&bndl, OLED);

	// delay(1000);

	// additional_loop_checks();			// Miscellaneous checks

	// // --- End Example ---



	// // --- LoRa Hub Example ---

	// receive_bundle(&bndl, LORA);		// Read sensors, store data in sensor state struct
	// print_bundle(&bndl);
	// log_bundle(&bndl, PUSHINGBOX);

	// additional_loop_checks();			// Miscellaneous checks
	// // --- End Example ---



	// // --- LoRa Node Example ---

	// measure_sensors();					// Read sensors, store data in sensor state struct
	// package_data(&bndl);			// Copy sensor data from state to provided bundle
	
	// print_bundle(&bndl);

	// log_bundle(&bndl, SDCARD, "savefile.csv");
	// send_bundle(&bndl, LORA);

	// delay(1000);

	// additional_loop_checks();			// Miscellaneous checks
	// // --- End Example ---



	// // --- LoRa Evaporometer Example ---

	// measure_sensors();					// Read sensors, store data in sensor state struct
	// package_data(&bndl);			// Copy sensor data from state to provided bundle
	
	// print_bundle(&bndl);

	// log_bundle(&bndl, SDCARD, "savefile.csv");
	// send_bundle(&bndl, LORA);

	// sleep_for(5, MINUTES, STANDBY);

	// additional_loop_checks();			// Miscellaneous checks
	// // --- End Example ---



	// // --- Common Example ---

	// receive_bundle(&bndl, WIFI);		// Receive messages
	// if (bndl.size()) {
	// 	print_bundle(&bndl);			// Print bundle if LOOM_DEBUG enabled
	// }
	// process_bundle(&bndl);				// Dispatch message to correct handling functions

	// measure_sensors();					// Read sensors, store data in sensor state struct
	// package_data(&bndl);			// Copy sensor data from state to provided bundle

	// // print_bundle(&bndl);			// Print bundle if LOOM_DEBUG enabled

	// send_bundle(&bndl, WIFI);		// Send bundle of packaged data
	// // log_bundle(&bndl, PUSHINGBOX);	// Send bundle to Google Sheet
	// // log_bundle(&bndl, SDCARD, "Ishield.csv");	// Send bundle to Google Sheet

	// additional_loop_checks();			// Miscellaneous checks
	// // --- End Example ---
}


// ================================================================ 
// ===                 High-Level API Functions                 === 
// ================================================================ 

// void receive_bundle(OSCBundle *bndl, CommPlatform platform);
// void process_bundle(OSCBundle *bndl);
// void measure_sensors();
// void package_data(OSCBundle *bndl);
// void send_bundle(OSCBundle *bndl, CommPlatform platform, int port);
// void send_bundle(OSCBundle *bndl, CommPlatform platform);
// void log_bundle(OSCBundle *bndl, LogPlatform platform, char* file); // filename required for SD files
// void log_bundle(OSCBundle *bndl, LogPlatform platform);
// bool bundle_empty(OSCBundle *bndl);
// void additional_loop_checks();
// void sleep_for(int amount, TimeUnits units, SleepMode mode);
// void sleep_until_time(SleepMode mode, int hour, int min, int sec);

// CommPlatforms: WIFI, LORA, NRF
// LogPlatforms:  PUSHINGBOX, SDCARD, OLED
// TimeUnits: MINUTES, SECONDS
// SleepMode: STANDBY, SLEEPYDOG
