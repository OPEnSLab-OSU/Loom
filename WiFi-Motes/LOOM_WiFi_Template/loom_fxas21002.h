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
	float gyro[3];
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
#if LOOM_DEBUG == 1
	void details_fxas21002();
#endif


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
bool setup_fxas21002() {
	bool is_setup;
	state_fxas21002.inst_fxas21002 = Adafruit_FXAS21002C(0x0021002C);
	
	if(state_fxas21002.inst_fxas21002.begin()) {
		is_setup = true;
		#if LOOM_DEBUG == 1
			Serial.println("Initialized fxas21002.");
		#endif
	}
	else {
		is_setup = false;
		#if LOOM_DEBUG == 1
			Serial.println("Failed to initialize fxas21002.");
		#endif
	}
	return is_setup;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================
void package_data_fxas21002(OSCBundle *bndl, char packet_header_string[]) {
	char address_string[255];
	sprintf(address_string, "%s%s", packet_header_string, "/fxas21002_data");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add("gx").add(state_fxas21002.gyro[0]);
	msg.add("gy").add(state_fxas21002.gyro[1]);
	msg.add("gz").add(state_fxas21002.gyro[2]);
	
	bndl->add(msg);
}


void measure_fxas21002() {
	/* Get a new sensor event */
  sensors_event_t event;
  state_fxas21002.inst_fxas21002.getEvent(&event);

  state_fxas21002.gyro[0] = event.gyro.x;
  state_fxas21002.gyro[1] = event.gyro.y;
  state_fxas21002.gyro[2] = event.gyro.z;
	
	#if LOOM_DEBUG == 1
		Serial.print("X: "); Serial.print(state_fxas21002.gyro[0]); Serial.print("  ");
		Serial.print("Y: "); Serial.print(state_fxas21002.gyro[1]); Serial.print("  ");
		Serial.print("Z: "); Serial.print(state_fxas21002.gyro[2]); Serial.print("  ");
		Serial.println("rad/s ");
	#endif
}


#if LOOM_DEBUG == 1
void details_fxas21002() {
	sensor_t sensor;
	state_fxas21002.inst_fxas21002.getSensor(&sensor);
	Serial.println("------------------------------------");
	Serial.print  ("Sensor:       ");   Serial.println(sensor.name);
	Serial.print  ("Driver Ver:   ");   Serial.println(sensor.version);
	Serial.print  ("Unique ID:    0x"); Serial.println(sensor.sensor_id, HEX);
	Serial.print  ("Max Value:    ");   Serial.print(sensor.max_value);  Serial.println(" rad/s");
	Serial.print  ("Min Value:    ");   Serial.print(sensor.min_value);  Serial.println(" rad/s");
	Serial.print  ("Resolution:   ");   Serial.print(sensor.resolution); Serial.println(" rad/s");
	Serial.println("------------------------------------");
	Serial.println("");
	delay(500);
}
#endif
