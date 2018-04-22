// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXAS21002C.h>


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define i2c_addr_fxas21002 0x20 //0x20, 0x21

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct config_fxas21002_t {

};

struct state_fxas21002_t {
	Adafruit_FXAS21002C inst_fxas21002;
	float x;
	float y;
	float z;
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_fxas21002_t config_fxas21002;
struct state_fxas21002_t state_fxas21002;

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_fxas21002();
void package_data_fxas21002(OSCBundle *, char[]);
void measure_fxas21002();
#if DEBUG == 1
	void details_fxas21002();
#endif



// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
bool setup_fxas21002() {
	bool is_setup;
	state_fxas21002.inst_fxas21002 = Adafruit_FXAS21002C(0x0021002C);
	
	if(state_fxas21002.inst_fxas21002.begin()) {
		#if DEBUG == 1
			Serial.println("Initialized fxas21002.");
		#endif
	}
	else {
		#if DEBUG == 1
			Serial.println("Failed to initialize fxas21002.");
		#endif
	}
	
	return is_setup;
}

// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================
void package_data_fxas21002(OSCBundle *bndl, char packet_header_string[]) {
	//Create a message and fill it here, then add it to the bndl
}

void measure_fxas21002() {
	/* Get a new sensor event */
  sensors_event_t event;
  state_fxas21002.inst_fxas21002.getEvent(&event);

  state_fxas21002.x = event.gyro.x;
  state_fxas21002.y = event.gyro.y;
  state_fxas21002.z = event.gyro.z;
	
	#if DEBUG == 1
		Serial.print("X: "); Serial.print(state_fxas21002.x); Serial.print("  ");
		Serial.print("Y: "); Serial.print(state_fxas21002.y); Serial.print("  ");
		Serial.print("Z: "); Serial.print(state_fxas21002.z); Serial.print("  ");
		Serial.println("rad/s ");
	#endif
}

#if DEBUG == 1
	void details_fxas21002() {
		sensor_t sensor;
		state_fxas21002.inst_fxas21002.getSensor(&sensor);
		Serial.println("------------------------------------");
		Serial.print  ("Sensor:       "); Serial.println(sensor.name);
		Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
		Serial.print  ("Unique ID:    0x"); Serial.println(sensor.sensor_id, HEX);
		Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" rad/s");
		Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" rad/s");
		Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" rad/s");
		Serial.println("------------------------------------");
		Serial.println("");
		delay(500);
	}
#endif
