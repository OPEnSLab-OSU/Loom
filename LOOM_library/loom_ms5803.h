// ================================================================ 
// ===                ACCESSSING SENSOR STRUCTS                 === 
// ================================================================

// state_ms5803_0x76.temp
// state_ms5803_0x76.pressure

// state_ms5803_0x77.temp
// state_ms5803_0x77.pressure


// ================================================================ 
// ===                          NOTES                           === 
// ================================================================

// Code found at: https://github.com/millerlp/MS5803_02

// CSB (Pin 3) Should be tied to 3.3V to set I2C Addr to 0x76
// PS (Pin 6) Needs to be tied to 3.3V to enable I2C

// Units:
// - Pressure: mbar
// - Temperature: Celcius
//

// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include <MS5803_02.h> 


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
// #define i2c_addr_ms5803 0x76	// 0x76 if CSB (pin 3) is High, 0x77 if CSB is Low



// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================

struct state_ms5803_t {
	float 			temp;		//compensated, final temperature
	float 			pressure;	//compensated, final pressure
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

// Declare 'sensor' as the object that will refer to your MS5803 in the sketch
// Enter the oversampling value as an argument. Valid choices are
// 256, 512, 1024, 2048, 4096. Library default = 512.

#if i2c_addr_ms5803_0x76 == 1
	struct state_ms5803_t state_ms5803_0x76;
	MS_5803 sensor_0x76 = MS_5803(512);
#endif
#if i2c_addr_ms5803_0x77 == 1
	struct state_ms5803_t state_ms5803_0x77;
	MS_5803 sensor_0x77 = MS_5803(512);
#endif

uint16_t MS_PROM[8]; //storing calibration data


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_ms5803();
void package_ms5803(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void package_ms5803(OSCBundle *bndl, char packet_header_string[]);
void measure_ms5803();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any MS5803 setup
//
// @return  Whether or not sensor initialization was successful
//
bool setup_ms5803() 
{
	LOOM_DEBUG_Println("Initializing MS_5803 pressure sensor");

  // Initialize the MS5803 sensor. This will report the
  // conversion coefficients to the Serial terminal if present.
  // If you don't want all the coefficients printed out, 
  // set sensor.initializeMS_5803(false).
	LOOM_DEBUG_Println("MS5803 conversion coefficients:");

	#if i2c_addr_ms5803_0x76 == 1
		if (sensor_0x76.initializeMS_5803()) {
			LOOM_DEBUG_Println("MS5803 CRC check OK.");
		} else {
			LOOM_DEBUG_Println("MS5803 CRC check FAILED!");
			LOOM_DEBUG_Println("(but it might work anyway...)");
		}
	#endif

	#if i2c_addr_ms5803_0x77 == 1
		if (sensor_0x77.initializeMS_5803()) {
			LOOM_DEBUG_Println("MS5803 CRC check OK.");
		} else {
			LOOM_DEBUG_Println("MS5803 CRC check FAILED!");
			LOOM_DEBUG_Println("(but it might work anyway...)");
		}
	#endif


	delay(3000);

	LOOM_DEBUG_Println("Initialized MS_5803 pressure sensor");
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE MS5803 --- (Multiplexer Version)
// 
// Adds last read MS5803 sensor readings to provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
// @param port                  Which port of the multiplexer the device is plugged into
//
void package_ms5803(OSCBundle *bndl, char packet_header_string[], uint8_t port)
{
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/ms5803/data");
	
	OSCMessage msg = OSCMessage(address_string);

	#if i2c_addr_ms5803_0x76 == 1
		msg.add("pressure").add(state_ms5803_0x76.pressure);
		msg.add("temp").add(state_ms5803_0x76.temp);
	#endif
	#if i2c_addr_ms5803_0x77 == 1
		msg.add("pressure").add(state_ms5803_0x77.pressure);
		msg.add("temp").add(state_ms5803_0x77.temp);
	#endif

	bndl->add(msg);
}

#if is_multiplexer != 1
void package_ms5803(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	#if i2c_addr_ms5803_0x76 == 1
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", ms5803_0x76_name, "_pressure");
		bndl->add(address_string ).add(state_ms5803_0x76.pressure);
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", ms5803_0x76_name, "_temp");
		bndl->add(address_string).add(state_ms5803_0x76.temp);
	#endif

	#if i2c_addr_ms5803_0x77 == 1
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", ms5803_0x77_name, "_pressure");
		bndl->add(address_string).add(state_ms5803_0x77.pressure);
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", ms5803_0x77_name, "_temp");
		bndl->add(address_string).add(state_ms5803_0x77.temp);
	#endif
}
#endif

// --- MEASURE MS5803 ---
//
// Gets the current sensor readings of the MS5803 and stores into its state struct
// 
void measure_ms5803() 
{
	// Get sensor readings

	#if i2c_addr_ms5803_0x76 == 1
		sensor_0x76.readSensor();

		state_ms5803_0x76.pressure = sensor_0x76.pressure();
		state_ms5803_0x76.temp = sensor_0x76.temperature();

		#if LOOM_DEBUG == 1
			Serial.print("MS5803 Pressure: ");     Serial.print(state_ms5803_0x76.pressure,  4);	Serial.println(" mbar");
			Serial.print("MS5803 Temperature: ");  Serial.print(state_ms5803_0x76.temp,  4);		Serial.println(" C");
		#endif
	#endif


	#if i2c_addr_ms5803_0x77 == 1
		sensor_0x77.readSensor();

		state_ms5803_0x77.pressure = sensor_0x77.pressure();
		state_ms5803_0x77.temp = sensor_0x77.temperature();

		#if LOOM_DEBUG == 1
			Serial.print("MS5803 Pressure: ");     Serial.print(state_ms5803_0x77.pressure,  4);	Serial.println(" mbar");
			Serial.print("MS5803 Temperature: ");  Serial.print(state_ms5803_0x77.temp,  4);		Serial.println(" C");
		#endif
	#endif
}


