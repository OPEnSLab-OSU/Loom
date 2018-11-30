// ================================================================ 
// ===                ACCESSSING SENSOR STRUCTS                 === 
// ================================================================

// state_analog.a0
// state_analog.a1
// state_analog.a2
// state_analog.a3
// state_analog.a4
// state_analog.a5


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define analog_resolution 12	// How many bits to read (normally defaults to 10, but M0 supports 12)
								//   Other devices will just zero pad the least significant bits beyond the supported number of bits

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct state_analog_t {
	// floats to support analog conversion functions
	float a0, a1, a2, a3, a4, a5; // Memory to store analog sensor values
													// Add other ports ? put these in #defines based on which ports are enabled
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct state_analog_t state_analog;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_analog();
int  read_analog(uint8_t chnl);
void measure_analog();
void package_analog(OSCBundle *bndl, char packet_header_string[]);


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================

void setup_analog() {
	#ifdef is_m0
		analogReadResolution(analog_resolution);
	#endif

	#if is_analog_a0 == 1
		pinMode(A0, INPUT);
	#endif
	#if is_analog_a1 == 1
		pinMode(A1, INPUT);
	#endif
	#if is_analog_a2 == 1
		pinMode(A2, INPUT);
	#endif
	#if is_analog_a3 == 1
		pinMode(A3, INPUT);
	#endif
	#if is_analog_a4 == 1
		pinMode(A4, INPUT);
	#endif
	#if is_analog_a5 == 1
		pinMode(A5, INPUT);
	#endif
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================



// --- READ ANALOG ---
//
// Generic subroutine for reading raw sensor data with averaging
// Called by measure analog
//
// @param chnl  The port to read from
//
// @return The measured sensor value
//
int read_analog(uint8_t chnl)
{
	int i = analog_samples;
	int reading = 0;

	while (i--) {
		reading += analogRead(chnl); 
	}

	#if (analog_samples == 1) 	// Take a reading    
		return (reading);
	#endif
	#if (analog_samples == 2) 	// Take a reading    
		return (reading >> 1);  // Then divide by 2 to get average sample value
	#endif
	#if (analog_samples == 4) 	// Take a reading    
		return (reading >> 2);  // Then divide by 4 to get average sample value
	#endif
	#if (analog_samples == 8) 	// Take a reading    
		return (reading >> 3);	// Then divide by 8 to get average sample value
	#endif
	#if (analog_samples == 16) 	// Take a reading    
		return (reading >> 4); 	// Then divide by 16 to get average sample value
	#endif
} 



// --- MEASURE ANALOG ---
//
// Measure analog data and update analog state to most recent readings. 
//
void measure_analog() 
{
	#if (is_analog_a0 == 1) 
		#if (enable_analog_conversions == 1) && (analog_a0_conversion >= 0)
			state_analog.a0 = conversion_list[analog_a0_conversion](read_analog(0)); 
		#else
			state_analog.a0 = read_analog(0);
		#endif
	#endif
	#if (is_analog_a1 == 1)
		#if (enable_analog_conversions == 1) && (analog_a1_conversion >= 0)
			state_analog.a1 = conversion_list[analog_a1_conversion](read_analog(1)); 
		#else
			state_analog.a1 = read_analog(1);
		#endif
	#endif
	#if (is_analog_a2 == 1) 
		#if (enable_analog_conversions == 1) && (analog_a2_conversion >= 0)
			state_analog.a2 = conversion_list[analog_a2_conversion](read_analog(2)); 
		#else
			state_analog.a2 = read_analog(2);
		#endif
	#endif
	#if (is_analog_a3 == 1)
		#if (enable_analog_conversions == 1) && (analog_a3_conversion >= 0)
			state_analog.a3 = conversion_list[analog_a3_conversion](read_analog(3)); 
		#else
			state_analog.a3 = read_analog(3);
		#endif
	#endif
	#if (is_analog_a4 == 1) 
		#if (enable_analog_conversions == 1) && (analog_a4_conversion >= 0)
			state_analog.a4 = conversion_list[analog_a4_conversion](read_analog(4)); 
		#else
			state_analog.a4 = read_analog(4);
		#endif
	#endif
	#if (is_analog_a5 == 1)
		#if (enable_analog_conversions == 1) && (analog_a5_conversion >= 0)
			state_analog.a5 = conversion_list[analog_a5_conversion](read_analog(5)); 
		#else
			state_analog.a5 = read_analog(5);
		#endif
	#endif
}



// --- PACKAGE ANALOG ---
//
// Gets analog reading from ports, as enabled,
// and forms an OSC bundle out of them.
//
// @param bndl                  The OSC bundle to be populated
// @param packet_header_string  The device-identifying string to prepend to OSC messages
//
void package_analog(OSCBundle *bndl, char packet_header_string[])
{ 
	char address_string[80];

	// Get readings from enabled ports           
	// #if (is_analog_a0 == 1) 
	// 	sprintf(address_string, "%s%s", packet_header_string, "/pin_A0");
	// 	bndl->add(address_string).add((int32_t)state_analog.a0);
	// #endif
	// #if (is_analog_a1 == 1)
	// 	sprintf(address_string, "%s%s", packet_header_string, "/pin_A1");
	// 	bndl->add(address_string).add((int32_t)state_analog.a1);
	// #endif
	// #if (is_analog_a2 == 1) 
	// 	sprintf(address_string, "%s%s", packet_header_string, "/pin_A2");
	// 	bndl->add(address_string).add((int32_t)state_analog.a2);
	// #endif
	// #if (is_analog_a3 == 1)
	// 	sprintf(address_string, "%s%s", packet_header_string, "/pin_A3");
	// 	bndl->add(address_string).add((int32_t)state_analog.a3);
	// #endif
	// #if (is_analog_a4 == 1) 
	// 	sprintf(address_string, "%s%s", packet_header_string, "/pin_A4");
	// 	bndl->add(address_string).add((int32_t)state_analog.a4);
	// #endif
	// #if (is_analog_a5 == 1)
	// 	sprintf(address_string, "%s%s", packet_header_string, "/pin_A5");
	// 	bndl->add(address_string).add((int32_t)state_analog.a5);
	// #endif
	#if (is_analog_a0 == 1) 
		sprintf(address_string, "%s%s", packet_header_string, "/pin_A0");
		bndl->add(address_string).add(state_analog.a0);
	#endif
	#if (is_analog_a1 == 1)
		sprintf(address_string, "%s%s", packet_header_string, "/pin_A1");
		bndl->add(address_string).add(state_analog.a1);
	#endif
	#if (is_analog_a2 == 1) 
		sprintf(address_string, "%s%s", packet_header_string, "/pin_A2");
		bndl->add(address_string).add(state_analog.a2);
	#endif
	#if (is_analog_a3 == 1)
		sprintf(address_string, "%s%s", packet_header_string, "/pin_A3");
		bndl->add(address_string).add(state_analog.a3);
	#endif
	#if (is_analog_a4 == 1) 
		sprintf(address_string, "%s%s", packet_header_string, "/pin_A4");
		bndl->add(address_string).add(state_analog.a4);
	#endif
	#if (is_analog_a5 == 1)
		sprintf(address_string, "%s%s", packet_header_string, "/pin_A5");
		bndl->add(address_string).add(state_analog.a5);
	#endif
}




// Example code for adjusting analog reading resolution

// analogReadResolution(10);
// Serial.print("ADC 10-bit (default) : ");
// Serial.print(analogRead(A0));

// // change the resolution to 12 bits and read A0
// analogReadResolution(12);
// Serial.print(", 12-bit : ");
// Serial.print(analogRead(A0));

// // change the resolution to 16 bits and read A0
// analogReadResolution(16);
// Serial.print(", 16-bit : ");
// Serial.print(analogRead(A0));

// // change the resolution to 8 bits and read A0
// analogReadResolution(8);
// Serial.print(", 8-bit : ");
// Serial.println(analogRead(A0));






