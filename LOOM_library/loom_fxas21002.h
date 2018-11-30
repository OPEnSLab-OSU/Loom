// ================================================================ 
// ===                ACCESSSING SENSOR STRUCTS                 === 
// ================================================================

// state_fxas21002_0x21.gyro[0] // x 
// state_fxas21002_0x21.gyro[1] // y
// state_fxas21002_0x21.gyro[2] // z


// ================================================================ 
// ===                          NOTES                           === 
// ================================================================

// This sensor supports I2C addresses 0x20 and 0x21, but the Adafruit 
// breakout the Loom uses (pairs it with a fxos8700) does not allow the 
// I2C address to be changed without modifing the library 

// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXAS21002C.h>


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
// #define i2c_addr_fxas21002 0x21 //0x20, 0x21


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
// struct config_fxas21002_t {

// };

struct state_fxas21002_t {
	float gyro[3];
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

Adafruit_FXAS21002C inst_fxas21002_0x21;

struct state_fxas21002_t state_fxas21002_0x21;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_fxas21002();
void package_fxas21002(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void package_fxas21002(OSCBundle *bndl, char packet_header_string[]);
void measure_fxas21002();
#if LOOM_DEBUG == 1
	void details_fxas21002();
#endif


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any FXAS21002 setup and initialization
//
// @return  Whether or not sensor initialization was successful
//
bool setup_fxas21002() 
{
	bool is_setup;
	inst_fxas21002_0x21 = Adafruit_FXAS21002C(0x0021002C);
	
	if (inst_fxas21002_0x21.begin()) {
		is_setup = true;
		LOOM_DEBUG_Println("Initialized fxas21002.");
	}
	else {
		is_setup = false;
		LOOM_DEBUG_Println("Failed to initialize fxas21002.");
	}
	return is_setup;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE FXAS21002 ---
//
// Adds OSC Message of last read FXAS21002 sensor readings to provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
// @param port                  Which port of the multiplexer the device is plugged into
//
void package_fxas21002(OSCBundle *bndl, char packet_header_string[], uint8_t port) 
{
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/fxas21002/data");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add("gx").add(state_fxas21002_0x21.gyro[0]);
	msg.add("gy").add(state_fxas21002_0x21.gyro[1]);
	msg.add("gz").add(state_fxas21002_0x21.gyro[2]);
	
	bndl->add(msg);
}

#if is_multiplexer != 1
void package_fxas21002(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	sprintf(address_string, "%s%s%s%s", packet_header_string, "/", fxas21002_0x21_name, "_gx");
	bndl->add(address_string).add(state_fxas21002_0x21.gyro[0]);
	sprintf(address_string, "%s%s%s%s", packet_header_string, "/", fxas21002_0x21_name, "_gy");
	bndl->add(address_string).add(state_fxas21002_0x21.gyro[1]);
	sprintf(address_string, "%s%s%s%s", packet_header_string, "/", fxas21002_0x21_name, "_gz");
	bndl->add(address_string).add(state_fxas21002_0x21.gyro[2]);
}
#endif

// --- MEASURE FXAS21002 ---
//
// Gets the current sensor readings of the Fxas21002 and stores into its state struct
//
void measure_fxas21002() 
{
	/* Get a new sensor event */
	sensors_event_t event;
	inst_fxas21002_0x21.getEvent(&event);

	state_fxas21002_0x21.gyro[0] = event.gyro.x;
	state_fxas21002_0x21.gyro[1] = event.gyro.y;
	state_fxas21002_0x21.gyro[2] = event.gyro.z;
	
	#if LOOM_DEBUG == 1
		Serial.print("X: "); Serial.print(state_fxas21002_0x21.gyro[0]); Serial.print("  ");
		Serial.print("Y: "); Serial.print(state_fxas21002_0x21.gyro[1]); Serial.print("  ");
		Serial.print("Z: "); Serial.print(state_fxas21002_0x21.gyro[2]); Serial.print("  ");
		Serial.println("rad/s ");
	#endif
}



// --- DETAILS FXAS21002 ---
//
// With Loom debug enabled, allows for the printing of the FXAS21002 details to be 
// printed to the Serial monitor
//
#if LOOM_DEBUG == 1
void details_fxas21002() 
{
	sensor_t sensor;
	inst_fxas21002_0x21.getSensor(&sensor);
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



