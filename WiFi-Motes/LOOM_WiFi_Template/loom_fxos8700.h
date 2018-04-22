// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXOS8700.h>

// ================================================================ 
// ===                        DEFINITIONS                       === 
// ================================================================ 
#define i2c_addr_fxos8700 0x1C 						 //0x1C, 0x1D, 0x1E, 0x1F


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 
struct config_fxos8700_t {

};

struct state_fxos8700_t {
	Adafruit_FXOS8700 inst_fxos8700;
	float accel[3];
	float mag[3];

};

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_fxos8700();
void package_data_fxos8700(OSCBundle *, char[]);
void measure_fxos8700();
#if DEBUG == 1
	void details_fxos8700();
#endif

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_fxos8700_t config_fxos8700;
struct state_fxos8700_t state_fxos8700;



// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
bool setup_fxos8700() {
  bool is_setup;
	state_fxos8700.inst_fxos8700 = Adafruit_FXOS8700(0x8700A, 0x8700B);
	if(state_fxos8700.inst_fxos8700.begin(ACCEL_RANGE_4G)) {
		is_setup = true;
		#if DEBUG == 1
			Serial.println("Initialzed fxos8700 (accel/mag)");
		#endif
	}
	else {
		is_setup = false;
		#if DEBUG == 1
		 Serial.println("Failed to initialize fxos8700 (accel/mag)");
		#endif
	}
	
	return is_setup;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================
void package_data_fxos8700(OSCBundle *bndl, char packet_header_string[]) {
	//Create a message and fill it here, then add it to the bndl
}

void measure_fxos8700() {
	sensors_event_t aevent, mevent;
  state_fxos8700.inst_fxos8700.getEvent(&aevent, &mevent);

  state_fxos8700.accel[0] = aevent.acceleration.x;
	state_fxos8700.accel[1] = aevent.acceleration.y;
	state_fxos8700.accel[2] = aevent.acceleration.z;

	state_fxos8700.mag[0] = mevent.magnetic.x;
	state_fxos8700.mag[1] = mevent.magnetic.y;
	state_fxos8700.mag[2] = mevent.magnetic.z;
	
	#if DEBUG == 1
		/* Display the accel results (acceleration is measured in m/s^2) */
		Serial.print("A ");
		Serial.print("X: "); Serial.print(state_fxos8700.accel[0], 4); Serial.print("  ");
		Serial.print("Y: "); Serial.print(state_fxos8700.accel[1], 4); Serial.print("  ");
		Serial.print("Z: "); Serial.print(state_fxos8700.accel[2], 4); Serial.print("  ");
		Serial.println("m/s^2");

		/* Display the mag results (mag data is in uTesla) */
		Serial.print("M ");
		Serial.print("X: "); Serial.print(state_fxos8700.mag[0], 1); Serial.print("  ");
		Serial.print("Y: "); Serial.print(state_fxos8700.mag[1], 1); Serial.print("  ");
		Serial.print("Z: "); Serial.print(state_fxos8700.mag[2], 1); Serial.print("  ");
		Serial.println("uT");

		Serial.println("");
	#endif 
}

#if DEBUG == 1
	void details_fxos8700() {
		sensor_t accel, mag;
		state_fxos8700.inst_fxos8700.getSensor(&accel, &mag);
		Serial.println("------------------------------------");
		Serial.println("ACCELEROMETER");
		Serial.println("------------------------------------");
		Serial.print  ("Sensor:       "); Serial.println(accel.name);
		Serial.print  ("Driver Ver:   "); Serial.println(accel.version);
		Serial.print  ("Unique ID:    0x"); Serial.println(accel.sensor_id, HEX);
		Serial.print  ("Min Delay:    "); Serial.print(accel.min_delay); Serial.println(" s");
		Serial.print  ("Max Value:    "); Serial.print(accel.max_value, 4); Serial.println(" m/s^2");
		Serial.print  ("Min Value:    "); Serial.print(accel.min_value, 4); Serial.println(" m/s^2");
		Serial.print  ("Resolution:   "); Serial.print(accel.resolution, 8); Serial.println(" m/s^2");
		Serial.println("------------------------------------");
		Serial.println("");
		Serial.println("------------------------------------");
		Serial.println("MAGNETOMETER");
		Serial.println("------------------------------------");
		Serial.print  ("Sensor:       "); Serial.println(mag.name);
		Serial.print  ("Driver Ver:   "); Serial.println(mag.version);
		Serial.print  ("Unique ID:    0x"); Serial.println(mag.sensor_id, HEX);
		Serial.print  ("Min Delay:    "); Serial.print(accel.min_delay); Serial.println(" s");
		Serial.print  ("Max Value:    "); Serial.print(mag.max_value); Serial.println(" uT");
		Serial.print  ("Min Value:    "); Serial.print(mag.min_value); Serial.println(" uT");
		Serial.print  ("Resolution:   "); Serial.print(mag.resolution); Serial.println(" uT");
		Serial.println("------------------------------------");
		Serial.println("");
		delay(500);
	}
#endif //if DEBUG == 1

