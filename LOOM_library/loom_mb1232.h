// ================================================================ 
// ===                ACCESSSING SENSOR STRUCTS                 === 
// ================================================================

// state_mb1232.high
// state_mb1232.low
// state_mb1232.range


// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>

// ================================================================
// ===                       DEFINITIONS                        ===
// ================================================================

// #define i2c_addr_mb1232 0x70 			



#define SCL_PIN 5 						//Default SDA is Pin5 PORTC for the UNO -- you can set this to any tristate pin
#define SCL_PORT PORTC
#define SDA_PIN 4 						//Default SCL is Pin4 PORTC for the UNO -- you can set this to any tristate pin
#define SDA_PORT PORTC
#define I2C_TIMEOUT 100 				//Define a timeout of 100 ms -- do not wait for clock stretching longer than this time 

#define RangeCommand    0x51			//The Sensor ranging command has a value of 0x51
#define ChangeAddressCommand1 0xAA		//These are the two commands that need to be sent in sequence to change the sensor address
#define ChangeAddressCommand2 0xA5


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
// struct config_mb1232_t {

// };

struct state_mb1232_t {
	byte high;
	byte low;
	int  range;
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
// struct config_mb1232_t config_mb1232;
struct state_mb1232_t  state_mb1232;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_mb1232();
void measure_mb1232();
void package_mb1232(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void package_mb1232(OSCBundle *bndl, char packet_header_string[]);


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any MB1232 setup
//
// @return  Whether or not sensor initialization was successful
//
bool setup_mb1232() {
	bool is_setup;
	
	// Wire.beginTransmission(i2c_addr_mb1232);
	Wire.beginTransmission(0x70);

	Wire.write(RangeCommand);
	Wire.endTransmission();
	delay(100);
	
	Wire.requestFrom(0x70, byte(2));
	if(Wire.available() >= 2) {
		is_setup = true;
		LOOM_DEBUG_Println("Initialized mb1232");
	}
	else {
		is_setup = false;
		LOOM_DEBUG_Println("Failed to initialize mb1232");
	}
	
	return is_setup;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE MB1232 ---
//
// Adds OSC Message of last read MB1232 sensor readings to provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
// @param port                  Which port of the multiplexer the device is plugged into
//
void package_mb1232(OSCBundle *bndl, char packet_header_string[], uint8_t port) 
{
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/mb1232/data");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add("range").add((int32_t)state_mb1232.range);

	bndl->add(msg);
}

#if is_multiplexer != 1
void package_mb1232(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	sprintf(address_string, "%s%s%s%s", packet_header_string, "/", mb1232_0x70_name, "_range");
	bndl->add(address_string).add((int32_t)state_mb1232.range);
}
#endif


// --- MEASURE MB1232 ---
//
// Gets the current sensor readings of the MB1232 and stores into its state struct
//
void measure_mb1232() 
{
	// Wire.beginTransmission(i2c_addr_mb1232);
	Wire.beginTransmission(0x70);

	Wire.write(RangeCommand);
	Wire.endTransmission();
	
	delay(100);
	
	Wire.requestFrom(0x70, byte(2));
	if (Wire.available() >= 2) {
		// The sensor communicates two bytes, each a range. The
		// high byte is typically zero, in which case the low
		// byte is equal to the range, so only the range is transmitted.
		// The low byte will not be less than 20.
		state_mb1232.high = Wire.read();
		state_mb1232.low = Wire.read();
		byte tmp = Wire.read();
		state_mb1232.range = (state_mb1232.high * 256) + state_mb1232.low;

		LOOM_DEBUG_Println2("High Byte: ", state_mb1232.high);
		LOOM_DEBUG_Println2("Low Byte: ",  state_mb1232.low);
		LOOM_DEBUG_Println2("Range:",      state_mb1232.range);
	} else {
		LOOM_DEBUG_Println("Error reading from mb1232 (range)");
	}       
}





