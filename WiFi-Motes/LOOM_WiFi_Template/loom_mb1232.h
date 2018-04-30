// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>

// ================================================================
// ===                       DEFINITIONS                        ===
// ================================================================
#define SCL_PIN 5 													//Default SDA is Pin5 PORTC for the UNO -- you can set this to any tristate pin
#define SCL_PORT PORTC
#define SDA_PIN 4 													//Default SCL is Pin4 PORTC for the UNO -- you can set this to any tristate pin
#define SDA_PORT PORTC
#define I2C_TIMEOUT 100 										//Define a timeout of 100 ms -- do not wait for clock stretching longer than this time 

#define i2c_addr_mb1232 0x70 								//The Sensor ranging command has a value of 0x51
#define RangeCommand 0x51
#define ChangeAddressCommand1 0xAA					//These are the two commands that need to be sent in sequence to change the sensor address
#define ChangeAddressCommand2 0xA5


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct config_mb1232_t {

};

struct state_mb1232_t {
	byte high;
	byte low;
	int  range;
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_mb1232_t config_mb1232;
struct state_mb1232_t state_mb1232;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_mb1232();
void measure_mb1232();
void package_mb1232(OSCBundle *bndl, char packet_header_string[], uint8_t port);

// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
bool setup_mb1232() {
	bool is_setup;
	
	Wire.beginTransmission(i2c_addr_mb1232);
  Wire.write(RangeCommand);
  Wire.endTransmission();
	delay(100);
	
	Wire.requestFrom(i2c_addr_mb1232, byte(2));
	if(Wire.available() >= 2) {
		is_setup = true;
		#if LOOM_DEBUG == 1
			Serial.println("Initialized mb1232");
		#endif
	}
	else {
		is_setup = false;
		#if LOOM_DEBUG == 1
			Serial.println("Failed to initialize mb1232");
		#endif
	}
	
	return is_setup;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================

void package_mb1232(OSCBundle *bndl, char packet_header_string[], uint8_t port) {
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/mb1232/data");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add("low").add((int32_t)state_mb1232.low);
	msg.add("high").add((int32_t)state_mb1232.high);
	msg.add("range").add((int32_t)state_mb1232.range);
	
	bndl->add(msg);
}

void measure_mb1232() {
	Wire.beginTransmission(i2c_addr_mb1232);
  Wire.write(RangeCommand);
  Wire.endTransmission();
	
	delay(100);
	
	Wire.requestFrom(i2c_addr_mb1232, byte(2));
	if(Wire.available() >= 2){
		state_mb1232.high = Wire.read();
		state_mb1232.low = Wire.read();
		byte tmp = Wire.read();
		state_mb1232.range = (state_mb1232.high * 256) + state_mb1232.low;
		#if LOOM_DEBUG == 1
			Serial.print("High Byte: ");
			Serial.println(state_mb1232.high);
			Serial.print("Low Byte: ");
			Serial.println(state_mb1232.low);
			Serial.print("Range: ");
			Serial.println(state_mb1232.range);
		#endif
	}
	else {
		#if LOOM_DEBUG == 1
			Serial.println("Error reading from mb1232 (range)");
		#endif
	}       
}
