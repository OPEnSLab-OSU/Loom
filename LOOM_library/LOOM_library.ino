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
	// receive_bundle(&bndl, WIFI);



	// Test key-value array to both bundle formats
	String key_values[10] = {"key1","0.0","key2","0","key3","1.234","key4","42","key5","0.9567"};
	int ints[10]; float floats[10]; char* c_strs[10]; String strings[10]; 
	convert_OSC_key_value_array_to_singleMsg(key_values, &bndl, "test/packet/header", 10);

	convert_OSC_to_array(&bndl, ints, 10);
	Serial.println("\nInts:"); 
	print_array(ints, 10, 1);
	convert_OSC_to_array(&bndl, floats, 10);
	Serial.println("\nFloats:"); 
	print_array(floats, 10, 1);
	convert_OSC_to_array(&bndl, c_strs, 10);
	Serial.println("\nC-strs:"); 
	print_array(c_strs, 10, 1);
	convert_OSC_to_array(&bndl, strings, 10);
	Serial.println("\nStrings:"); 
	print_array(strings, 10, 1);



	while(1);





	// Process bundle (nothing will happen if bndl is empty), bundle is emptied after processing
//	process_bundle(&bndl);

	// Update stored readings from sensors
	measure_sensors();

	// Populate bundle to send with sensor values
	package_data(&send_bndl);

	// Send the bundle, takes bundle to be filled and platform to send it over [WIFI, LORA, NRF, SDCARD, PUSHINGBOX]
	send_bundle(&send_bndl, LORA);
//
//	if (get_bundle_bytes(&bndl)){
//		send_bundle(&bndl, SDCARD, "test2.txt");
//		read_all_from_file("test2.txt");
////		while(1);
//	}
	
	// Loop checks and sleep between iterations if enabled
	additional_loop_checks();
	
} // End loop section





