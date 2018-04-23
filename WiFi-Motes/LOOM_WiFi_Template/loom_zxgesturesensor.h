// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include <ZX_Sensor.h>

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define i2c_addr_zxgesturesensor 0x10 								 //0x10, 0x11

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 
struct config_zxgesturesensor_t {

};

struct state_zxgesturesensor_t {
	ZX_Sensor inst_zxgesturesensor = ZX_Sensor(i2c_addr_zxgesturesensor);
	GestureType gesture;
	uint8_t gesture_speed;
	float pos[2];
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_zxgesturesensor_t config_zxgesturesensor;
struct state_zxgesturesensor_t state_zxgesturesensor;

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_zxgesturesensor();
void package_data_zxgesturesensor(OSCBundle *bndl, char packet_header_string[]);
void measure_zxgesturesensor();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
bool setup_zxgesturesensor() {
  bool is_setup;
	uint8_t ver;
	
	if(state_zxgesturesensor.inst_zxgesturesensor.init()) {
		is_setup = true;
		#if LOOM_DEBUG == 1
			Serial.println("Initialized zxgesturesensor");
		#endif
	}
	else {
		is_setup = false;
		#if LOOM_DEBUG == 1
			Serial.println("Failed to initialize zxgesturesensor");
		#endif
	}
	
	ver = state_zxgesturesensor.inst_zxgesturesensor.getModelVersion();
	if(ver != ZX_MODEL_VER) {
		is_setup = false;
		#if LOOM_DEBUG == 1
			Serial.println("Incorrect Model Version or unable to read Model Version.");
		#endif
	}
	else {
		#if LOOM_DEBUG == 1
			Serial.print("Model Version: ");
			Serial.println(ver);
		#endif
	}
  
  // Read the register map version and ensure the library will work
  ver = state_zxgesturesensor.inst_zxgesturesensor.getRegMapVersion();
	if(ver != ZX_REG_MAP_VER) {
		is_setup = false;
		#if LOOM_DEBUG == 1
			Serial.println("Incorrect Register Map Version or unable to read Register Map Version.");
		#endif
	}
	else {
		#if LOOM_DEBUG == 1
			Serial.print("Register Map Version: ");
			Serial.println(ver);
		#endif
	}
	
	return is_setup;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================
void package_data_zxgesturesensor(OSCBundle *bndl, char packet_header_string[]) {
	//Create a message and fill it here, then add it to the bndl
}

void measure_zxgesturesensor() {
	uint8_t x;
	uint8_t z;
	
	if(state_zxgesturesensor.inst_zxgesturesensor.positionAvailable()) {
    x = state_zxgesturesensor.inst_zxgesturesensor.readX();
		z = state_zxgesturesensor.inst_zxgesturesensor.readZ();
  }
	else {
		#if LOOM_DEBUG == 1
			Serial.println("Position data unavailable for zxgesturesensor");
		#endif
	}
	
	if((x != ZX_ERROR) && (z != ZX_ERROR)) {
		state_zxgesturesensor.pos[0] = x;
		state_zxgesturesensor.pos[1] = z;
		#if LOOM_DEBUG == 1
			Serial.print("zxgesturesensor X: ");
			Serial.println(state_zxgesturesensor.pos[0]);
			Serial.print("zxgesturesensor Z: ");
			Serial.println(state_zxgesturesensor.pos[1]);
		#endif
	}
	else {
		#if LOOM_DEBUG == 1
			Serial.println("Error occurred while reading position data");
		#endif
	}
	
	if(state_zxgesturesensor.inst_zxgesturesensor.gestureAvailable()) {
    state_zxgesturesensor.gesture = state_zxgesturesensor.inst_zxgesturesensor.readGesture();
    state_zxgesturesensor.gesture_speed = state_zxgesturesensor.inst_zxgesturesensor.readGestureSpeed();
		
		#if LOOM_DEBUG == 1
			switch (state_zxgesturesensor.gesture) {
				case NO_GESTURE:
						Serial.println("No Gesture");
					break;
				case RIGHT_SWIPE:
						Serial.println("Right Swipe");
					break;
				case LEFT_SWIPE:
						Serial.println("Left Swipe");
					break;
				case UP_SWIPE:
						Serial.println("Up Swipe");
				default:
					break;
			}
			Serial.println(state_zxgesturesensor.gesture_speed, DEC);
		#endif
  }
	else {
		#if LOOM_DEBUG == 1
			Serial.println("Gesture data unavailable for zxgesturesensor");
		#endif
	}
}

