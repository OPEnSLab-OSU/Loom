// ================================================================ 
// ===                ACCESSSING SENSOR STRUCTS                 === 
// ================================================================

// state_fxos8700_0x1F.accel[0] // x 
// state_fxos8700_0x1F.accel[1] // y
// state_fxos8700_0x1F.accel[2] // z
// state_fxos8700_0x1F.mag[0] // x 
// state_fxos8700_0x1F.mag[1] // y
// state_fxos8700_0x1F.mag[2] // z


// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXOS8700.h>

// ================================================================ 
// ===                        DEFINITIONS                       === 
// ================================================================ 
// #define i2c_addr_fxos8700 0x1C 						 //0x1C, 0x1D, 0x1E, 0x1F


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 

struct state_fxos8700_t {
	float accel[3];
	float mag[3];
};

#if i2c_addr_fxos8700_0x1C == 1
	Adafruit_FXOS8700 inst_fxos8700_0x1C;
#endif
#if i2c_addr_fxos8700_0x1D == 1
	Adafruit_FXOS8700 inst_fxos8700_0x1D;
#endif
#if i2c_addr_fxos8700_0x1E == 1
	Adafruit_FXOS8700 inst_fxos8700_0x1E;
#endif
#if i2c_addr_fxos8700_0x1F == 1
	Adafruit_FXOS8700 inst_fxos8700_0x1F;
#endif

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_fxos8700();
void package_fxos8700(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void package_fxos8700(OSCBundle *bndl, char packet_header_string[]);
void measure_fxos8700();
#if LOOM_DEBUG == 1
	void details_fxos8700();
#endif

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

// #if i2c_addr_fxos8700_0x1C == 1
// 	struct state_fxos8700_t state_fxos8700_0x1C;
// #endif
// #if i2c_addr_fxos8700_0x1D == 1
// 	struct state_fxos8700_t state_fxos8700_0x1D;
// #endif
// #if i2c_addr_fxos8700_0x1E == 1
// 	struct state_fxos8700_t state_fxos8700_0x1E;
// #endif
#if i2c_addr_fxos8700_0x1F == 1
	struct state_fxos8700_t state_fxos8700_0x1F;
#endif

// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any FXOS8700 setup and initialization
//
// @return  Whether or not sensor initialization was successful
//
bool setup_fxos8700() 
{
	bool is_setup;

	// #if i2c_addr_fxos8700_0x1C == 1
	// 	inst_fxos8700_0x1C = Adafruit_FXOS8700(0x8700A, 0x8700B);
	// 	if(inst_fxos8700_0x1C.begin(ACCEL_RANGE_4G)) {
	// 		is_setup = true;
	// 		LOOM_DEBUG_Println("Initialzed fxos8700 (accel/mag)");
	// 	} else {
	// 		is_setup = false;
	// 		LOOM_DEBUG_Println("Failed to initialize fxos8700 (accel/mag)");
	// 	}
	// #endif
	// #if i2c_addr_fxos8700_0x1D == 1
	// 	inst_fxos8700_0x1D = Adafruit_FXOS8700(0x8700A, 0x8700B);
	// 	if(inst_fxos8700_0x1D.begin(ACCEL_RANGE_4G)) {
	// 		is_setup = true;
	// 		LOOM_DEBUG_Println("Initialzed fxos8700 (accel/mag)");
	// 	} else {
	// 		is_setup = false;
	// 		LOOM_DEBUG_Println("Failed to initialize fxos8700 (accel/mag)");
	// 	}
	// #endif
	// #if i2c_addr_fxos8700_0x1E == 1
	// 	inst_fxos8700_0x1E = Adafruit_FXOS8700(0x8700A, 0x8700B);
	// 	if (inst_fxos8700_0x1E.begin(ACCEL_RANGE_4G)) {
	// 		is_setup = true;
	// 		LOOM_DEBUG_Println("Initialzed fxos8700 (accel/mag)");
	// 	} else {
	// 		is_setup = false;
	// 		LOOM_DEBUG_Println("Failed to initialize fxos8700 (accel/mag)");
	// 	}
	// #endif
	#if i2c_addr_fxos8700_0x1F == 1
		inst_fxos8700_0x1F = Adafruit_FXOS8700(0x8700A, 0x8700B);
		if (inst_fxos8700_0x1F.begin(ACCEL_RANGE_4G)) {
			is_setup = true;
			LOOM_DEBUG_Println("Initialzed fxos8700 (accel/mag)");
		} else {
			is_setup = false;
			LOOM_DEBUG_Println("Failed to initialize fxos8700 (accel/mag)");
		}
	#endif


	return is_setup;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE FXOS8700 ---
//
// Adds OSC Message of last read FXOS8700 sensor readings to provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
// @param port                  Which port of the multiplexer the device is plugged into
//
void package_fxos8700(OSCBundle *bndl, char packet_header_string[], uint8_t port) 
{
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/fxos8700/data");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add("ax").add(state_fxos8700_0x1F.accel[0]);
	msg.add("ay").add(state_fxos8700_0x1F.accel[1]);
	msg.add("az").add(state_fxos8700_0x1F.accel[2]);
	
	msg.add("mx").add(state_fxos8700_0x1F.mag[0]);
	msg.add("my").add(state_fxos8700_0x1F.mag[1]);
	msg.add("mz").add(state_fxos8700_0x1F.mag[2]);
	
	bndl->add(msg);
}

#if is_multiplexer != 1
void package_fxos8700(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	#if i2c_addr_fxos8700_0x1F == 1
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", Fxos8700_0x1F_name, "_gx");
		bndl->add(address_string).add(state_fxos8700_0x1F.accel[0]);
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", Fxos8700_0x1F_name, "_gy");
		bndl->add(address_string).add(state_fxos8700_0x1F.accel[1]);
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", Fxos8700_0x1F_name, "_gz");
		bndl->add(address_string).add(state_fxos8700_0x1F.accel[2]);
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", Fxos8700_0x1F_name, "_mx");
		bndl->add(address_string).add(state_fxos8700_0x1F.mag[0]);
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", Fxos8700_0x1F_name, "_my");
		bndl->add(address_string).add(state_fxos8700_0x1F.mag[1]);
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", Fxos8700_0x1F_name, "_mz");
		bndl->add(address_string).add(state_fxos8700_0x1F.mag[2]);
	#endif
}
#endif

// --- MEASURE FXOS8700 ---
//
// Gets the current sensor readings of the FXAS8700 and stores into its state struct
//
void measure_fxos8700() 
{
	sensors_event_t aevent, mevent;
	inst_fxos8700_0x1F.getEvent(&aevent, &mevent);

	state_fxos8700_0x1F.accel[0] = aevent.acceleration.x;
	state_fxos8700_0x1F.accel[1] = aevent.acceleration.y;
	state_fxos8700_0x1F.accel[2] = aevent.acceleration.z;

	state_fxos8700_0x1F.mag[0] = mevent.magnetic.x;
	state_fxos8700_0x1F.mag[1] = mevent.magnetic.y;
	state_fxos8700_0x1F.mag[2] = mevent.magnetic.z;
	
	#if LOOM_DEBUG == 1
		/* Display the accel results (acceleration is measured in m/s^2) */
		Serial.print("A ");
		Serial.print("X: "); Serial.print(state_fxos8700_0x1F.accel[0], 4); Serial.print("  ");
		Serial.print("Y: "); Serial.print(state_fxos8700_0x1F.accel[1], 4); Serial.print("  ");
		Serial.print("Z: "); Serial.print(state_fxos8700_0x1F.accel[2], 4); Serial.print("  ");
		Serial.println("m/s^2");

		/* Display the mag results (mag data is in uTesla) */
		Serial.print("M ");
		Serial.print("X: "); Serial.print(state_fxos8700_0x1F.mag[0], 1); Serial.print("  ");
		Serial.print("Y: "); Serial.print(state_fxos8700_0x1F.mag[1], 1); Serial.print("  ");
		Serial.print("Z: "); Serial.print(state_fxos8700_0x1F.mag[2], 1); Serial.print("  ");
		Serial.println("uT");

		Serial.println("");
	#endif 
}



// --- DETAILS FXOS8700 ---
//
// With Loom debug enabled, allows for the printing of the FX0S8700 details to be 
// printed to the Serial monitor
// 
#if LOOM_DEBUG == 1
void details_fxos8700() 
{
	sensor_t accel, mag;
	inst_fxos8700_0x1F.getSensor(&accel, &mag);
	Serial.println("------------------------------------");
	Serial.println("ACCELEROMETER");
	Serial.println("------------------------------------");
	Serial.print  ("Sensor:       ");   Serial.println(accel.name);
	Serial.print  ("Driver Ver:   ");   Serial.println(accel.version);
	Serial.print  ("Unique ID:    0x"); Serial.println(accel.sensor_id, HEX);
	Serial.print  ("Min Delay:    ");   Serial.print(accel.min_delay);     Serial.println(" s");
	Serial.print  ("Max Value:    ");   Serial.print(accel.max_value, 4);  Serial.println(" m/s^2");
	Serial.print  ("Min Value:    ");   Serial.print(accel.min_value, 4);  Serial.println(" m/s^2");
	Serial.print  ("Resolution:   ");   Serial.print(accel.resolution, 8); Serial.println(" m/s^2");
	Serial.println("------------------------------------");
	Serial.println("");
	Serial.println("------------------------------------");
	Serial.println("MAGNETOMETER");
	Serial.println("------------------------------------");
	Serial.print  ("Sensor:       ");   Serial.println(mag.name);
	Serial.print  ("Driver Ver:   ");   Serial.println(mag.version);
	Serial.print  ("Unique ID:    0x"); Serial.println(mag.sensor_id, HEX);
	Serial.print  ("Min Delay:    ");   Serial.print(accel.min_delay);   Serial.println(" s");
	Serial.print  ("Max Value:    ");   Serial.print(mag.max_value);     Serial.println(" uT");
	Serial.print  ("Min Value:    ");   Serial.print(mag.min_value);     Serial.println(" uT");
	Serial.print  ("Resolution:   ");   Serial.print(mag.resolution);    Serial.println(" uT");
	Serial.println("------------------------------------");
	Serial.println("");
	delay(500);
}
#endif //if LOOM_DEBUG == 1











