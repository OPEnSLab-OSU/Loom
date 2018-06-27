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

//	Serial.println(isdigit('a'));
//	Serial.println(isdigit('2'));
//	Serial.println(isdigit('Z'));
//	Serial.println(isdigit(7));
//	Serial.println("####");
//	Serial.println(atoi("a"));
//	Serial.println(atoi("7"));
//	Serial.println(atoi("Z"));
//	Serial.println(atoi("42"));
//	Serial.println(atoi("Z4S"));
//	Serial.println(atoi("0"));
//	Serial.println("####");
//	Serial.println(atof("a"));
//	Serial.println(atof("7"));
//	Serial.println(atof("3.0"));
//	Serial.println(atof("42.03"));
//	Serial.println(atof("1.1"));
//	Serial.println(atof("0"));

	const char *number;
	char *end;
	Serial.println("####");
	
	number = "7";
	Serial.print((int)strtol(number, &end, 10));
	if (end == number || *end != '\0')
		Serial.println(" *");
	else
		Serial.println();

	number = "abc";
	Serial.print((int)strtol(number, &end, 10));
	if (end == number || *end != '\0')
		Serial.println(" *");
	else
		Serial.println();

	number = "43.2";
	Serial.print((int)strtol(number, &end, 10));
	if (end == number || *end != '\0')
		Serial.println(" *");
	else
		Serial.println();

	number = "0.57";
	Serial.print((int)strtol(number, &end, 10));
	if (end == number || *end != '\0')
		Serial.println(" *");
	else
		Serial.println();

	number = "0";
	Serial.print((int)strtol(number, &end, 10));
	if (end == number || *end != '\0')
		Serial.println(" *");
	else
		Serial.println();


	Serial.println("####");
	
	number = "7";
	Serial.print(strtof(number, &end));
	if (end == number || *end != '\0')
		Serial.println(" *");
	else
		Serial.println();

	number = "abc";
	Serial.print(strtof(number, &end));
	if (end == number || *end != '\0')
		Serial.println(" *");
	else
		Serial.println();

	number = "43.2";
	Serial.print(strtof(number, &end));
	if (end == number || *end != '\0')
		Serial.println(" *");
	else
		Serial.println();

	number = "0.57";
	Serial.print(strtof(number, &end));
	if (end == number || *end != '\0')
		Serial.println(" *");
	else
		Serial.println();

	number = "0";
	Serial.print(strtof(number, &end));
	if (end == number || *end != '\0')
		Serial.println(" *");
	else
		Serial.println();

	while(1);

	// Test key-value array to both bundle formats
	String key_values[10], keys[5], values[5];
	char buf[10];
	for (int i = 0; i < 5; i++) {
		sprintf(buf, "id%d", i);
		key_values[i*2]   = buf;
		sprintf(buf, "data%d", i);
		key_values[i*2+1] = buf;
	}

	// Print before conversion
	LOOM_DEBUG_Println("Before conversion:");
	for (int i = 0; i < 10; i+=2) {
		LOOM_DEBUG_Print4("key_values[", i, ",", i+1);
		LOOM_DEBUG_Print4("]= ", key_values[i], ", ", key_values[i+1]);
		LOOM_DEBUG_Print4("\t | keys[", i/2, "]= ", keys[i/2]);
		LOOM_DEBUG_Println4("\t | values[", i/2, "]= ", values[i/2]);
	}

	convert_OSC_key_value_array_to_singleMsg(key_values, &bndl, "some_packet_header", 10);
	LOOM_DEBUG_Println("\nSingle Message Bundle:");
	print_bundle(&bndl);

	convert_OSC_key_value_array_to_multiMsg(key_values, &bndl, "some_packet_header", 10);
	LOOM_DEBUG_Println("\nMulti Message Bundle");
	print_bundle(&bndl);



	// Test assoc arrays to both bundle formats
	bndl.empty(); send_bndl.empty();
	String key_values2[10], keys2[5], values2[5];
	for (int i = 0; i < 5; i++) {
		sprintf(buf, "id%d", i);
		keys2[i]   = buf;
		sprintf(buf, "data%d", i);
		values2[i] = buf;
	}

	// Print before conversion
	LOOM_DEBUG_Println("Before conversion:");
	for (int i = 0; i < 10; i+=2) {
		LOOM_DEBUG_Print4("key_values[", i, ",", i+1);
		LOOM_DEBUG_Print4("]= ", key_values2[i], ", ", key_values2[i+1]);
		LOOM_DEBUG_Print4("\t | keys[", i/2, "]= ", keys2[i/2]);
		LOOM_DEBUG_Println4("\t | values[", i/2, "]= ", values2[i/2]);
	}

	convert_OSC_assoc_arrays_to_singleMsg(keys2, values2, &bndl, "some_packet_header", 5);
	LOOM_DEBUG_Println("\nSingle Message Bundle:");
	print_bundle(&bndl);

	convert_OSC_assoc_arrays_to_multiMsg(keys2, values2, &bndl, "some_packet_header", 5);
	LOOM_DEBUG_Println("\nMulti Message Bundle");
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






















