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
	String key_values[10], keys[5], values[5];
	char buf[10];
//	for (int i = 0; i < 5; i++) {
//		sprintf(buf, "id%d", i);
//		key_values[i*2]   = buf;
//		sprintf(buf, "data%d", i);
//		key_values[i*2+1] = buf;
//	}
	key_values[0] = "key1";
	key_values[1] = "0.0";
	key_values[2] = "key2";
	key_values[3] = "0";
	key_values[4] = "key3";
	key_values[5] = "1.234";
	key_values[6] = "key4";
	key_values[7] = "42";
	key_values[8] = "5";
	key_values[9] = "0.9567";


	convert_OSC_key_value_array_to_singleMsg(key_values, &bndl, "some_packet_header", 10, 0);
	LOOM_DEBUG_Println("\n\nSingle Message Bundle:");
	print_bundle(&bndl);

	convert_OSC_key_value_array_to_multiMsg(key_values, &bndl, "/some_packet_header", 10, 0);
	LOOM_DEBUG_Println("\n\nMulti Message Bundle");
	print_bundle(&bndl);

	
	// Test assoc arrays to both bundle formats
	bndl.empty(); send_bndl.empty();
	keys[0] = "key1";
	values[0] = "0.0";
	keys[1] = "key2";
	values[1] = "0";
	keys[2] = "key3";
	values[2] = "1.234";
	keys[3] = "key4";
	values[3] = "42";
	keys[4] = "5.0";
	values[4] = "0.9567";

	convert_OSC_assoc_arrays_to_singleMsg(keys, values, &bndl, "/some_packet_header", 5, 0);
	LOOM_DEBUG_Println("\n\nSingle Message Bundle:");
	print_bundle(&bndl);

	convert_OSC_assoc_arrays_to_multiMsg(keys, values, &bndl, "some_packet_header", 5, 0);
	LOOM_DEBUG_Println("\n\nMulti Message Bundle");
	print_bundle(&bndl);

	while(1);





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






























