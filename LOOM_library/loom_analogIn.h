// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define num_measurements 4      // Must be 1, 2, 4, or 8 number of analog measurements to sample and average per channel
#define analog_resolution 12	// How many bits to read (normally defaults to 10, but M0 supports 12)
								//   Other devices will just zero pad the least significant bits beyond the supported number of bits

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct state_analog_t {
	int16_t a0, a1, a2, a3, a4, a5; // Memory to store analog sensor values
													// Add other ports ? put these in #defines based on which ports are enabled
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct state_analog_t state_analog;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void     setup_analog();
uint32_t read_analog(uint8_t chnl);
void     measure_analog();
void     package_analog(OSCBundle *bndl, char packet_header_string[]);


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================

void setup_analog() {
	analogReadResolution(analog_resolution);
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
uint32_t read_analog(uint8_t chnl)
{
	int i = num_measurements;
	uint32_t reading = 0;

	while (i--) {
		reading += analogRead(chnl);
	}

	#if (num_measurements == 1) // Take a reading    
		return (reading);
	#endif
	#if (num_measurements == 2) // Take a reading    
		return (reading >> 1);    // Then divide by 2 to get average sample value
	#endif
	#if (num_measurements == 4) // Take a reading    
		return (reading >> 2);    // Then divide by 4 to get average sample value
	#endif
	#if (num_measurements == 8) // Take a reading    
		return (reading >> 3);    // Then divide by 8 to get average sample value
	#endif
} 



// --- MEASURE ANALOG ---
//
// Measure analog data and update analog state to most recent readings. 
//
void measure_analog() 
{
	#if (num_analog > 0) 
		state_analog.a0 = read_analog(0);
	#endif
	#if (num_analog > 1)
		state_analog.a1 = read_analog(1);
	#endif
	#if (num_analog > 2)
		state_analog.a2 = read_analog(2);
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
	char addressString[255];
	// Get readings from enabled ports           
	#if (num_analog > 0) 
		sprintf(addressString, "%s%s", packet_header_string, "/port0");
		bndl->add(addressString).add((int32_t)state_analog.a0);
	#endif
	#if (num_analog > 1)
		sprintf(addressString, "%s%s", packet_header_string, "/port1");
		bndl->add(addressString).add((int32_t)state_analog.a1);
	#endif
	#if (num_analog > 2)
		sprintf(addressString, "%s%s", packet_header_string, "/port2");
		bndl->add(addressString).add((int32_t)state_analog.a2);
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






