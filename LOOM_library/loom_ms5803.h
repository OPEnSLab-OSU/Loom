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
#define i2c_addr_ms5803 0x76	// 0x76 if CSB (pin 3) is High, 0x77 if CSB is Low



// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================

struct state_ms5803_t {
	float 			temp;		//compensated, final temperature
	// unsigned long 	tempNC;		//uncompensated, raw temp value
	float 			pressure;	//compensated, final pressure
	// unsigned long 	pressureNC;	//uncompensated, raw pressure value
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

// Declare 'sensor' as the object that will refer to your MS5803 in the sketch
// Enter the oversampling value as an argument. Valid choices are
// 256, 512, 1024, 2048, 4096. Library default = 512.
MS_5803 sensor = MS_5803(512);

struct state_ms5803_t state_ms5803;

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
	if (sensor.initializeMS_5803()) {
		LOOM_DEBUG_Println("MS5803 CRC check OK.");
	} else {
		LOOM_DEBUG_Println("MS5803 CRC check FAILED!");
		LOOM_DEBUG_Println("(but it might work anyway...)");
	}
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
	msg.add("pressure").add(state_ms5803.pressure);
	msg.add("temp").add(state_ms5803.temp);
	
	bndl->add(msg);
}

void package_ms5803(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	sprintf(addressString, "%s%s", packet_header_string, "/ms5803_pressure");
	bndl->add(addressString ).add(state_ms5803.pressure);
	sprintf(addressString, "%s%s", packet_header_string, "/ms5803_temp");
	bndl->add(addressString).add(state_ms5803.temp);
}


// --- MEASURE MS5803 ---
//
// Gets the current sensor readings of the MS5803 and stores into its state struct
// 
void measure_ms5803() 
{
	// Get sensor readings
	sensor.readSensor();

	// LOOM_DEBUG_Println("D1 = ", sensor.D1val());
	// LOOM_DEBUG_Println("D2 = ", sensor.D2val());

	state_ms5803.pressure = sensor.pressure();
	state_ms5803.temp = sensor.temperature();

	#if LOOM_DEBUG == 1
		Serial.print("MS5803 Pressure: ");     Serial.print(state_ms5803.pressure,  4);	Serial.println(" mbar");
		Serial.print("MS5803 Temperature: ");  Serial.print(state_ms5803.temp,  4);		Serial.println(" C");
	#endif
}


