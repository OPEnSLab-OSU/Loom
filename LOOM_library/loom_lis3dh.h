// ================================================================ 
// ===                ACCESSSING SENSOR STRUCTS                 === 
// ================================================================

// state_lis3dh.accel_x
// state_lis3dh.accel_y
// state_lis3dh.accel_z


// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include "SparkFunLIS3DH.h"

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
// #define i2c_addr_lis3dh 0x19	


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct config_lis3dh_t {

};

struct state_lis3dh_t {
	float accel_x, accel_y, accel_z;
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
// struct config_lis3dh_t config_lis3dh;
struct state_lis3dh_t state_lis3dh;
LIS3DH inst_lis3dh(I2C_MODE, 0x19); //Default constructor is I2C, addr 0x19.

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_lis3dh();
void package_lis3dh(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void package_lis3dh(OSCBundle *bndl, char packet_header_string[]);
void measure_lis3dh();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any LIS3DH setup
//
// @return  Whether or not sensor initialization was successful
//
bool setup_lis3dh() 
{
	inst_lis3dh.settings.adcEnabled      = 1;
	inst_lis3dh.settings.tempEnabled     = 1;
	inst_lis3dh.settings.accelSampleRate = 50;  //Hz.  Can be: 0,1,10,25,50,100,200,400,1600,5000 Hz
	inst_lis3dh.settings.accelRange      = 16;  //Max G force readable.  Can be: 2, 4, 8, 16
	inst_lis3dh.settings.xAccelEnabled   = 1;
	inst_lis3dh.settings.yAccelEnabled   = 1;
	inst_lis3dh.settings.zAccelEnabled   = 1;
  
	inst_lis3dh.begin();
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE LIS3DH --- (Multiplexer Version)
// 
// Adds last read LIS3DH sensor readings to provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
// @param port                  Which port of the multiplexer the device is plugged into
//
void package_lis3dh(OSCBundle *bndl, char packet_header_string[], uint8_t port)
{
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/lis3dh/data");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add("accel_x").add(state_lis3dh.accel_x);
	msg.add("accel_y").add(state_lis3dh.accel_y);
	msg.add("accel_z").add(state_lis3dh.accel_z);
	
	bndl->add(msg);
}

#if is_multiplexer != 1
void package_lis3dh(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	sprintf(address_string, "%s%s%s%s", packet_header_string, "/", lis3dh_0x19_name, "_accel_x");
	bndl->add(address_string).add(state_lis3dh.accel_x);
	sprintf(address_string, "%s%s%s%s", packet_header_string, "/", lis3dh_0x19_name, "_accel_y");
	bndl->add(address_string).add(state_lis3dh.accel_y);
	sprintf(address_string, "%s%s%s%s", packet_header_string, "/", lis3dh_0x19_name, "_accel_z");
	bndl->add(address_string).add(state_lis3dh.accel_z);
}
#endif

// --- MEASURE LIS3DH ---
//
// Gets the current sensor readings of the LIS3DH and stores into its state struct
// 
void measure_lis3dh() 
{
	float x = inst_lis3dh.readFloatAccelX();
	float y = inst_lis3dh.readFloatAccelY();
	float z = inst_lis3dh.readFloatAccelZ();

	#if LOOM_DEBUG == 1
		Serial.print("LIS3DH Accel X: "); Serial.println(x, 4);
		Serial.print("LIS3DH Accel Y: "); Serial.println(y, 4);
		Serial.print("LIS3DH Accel Z: "); Serial.println(z, 4);
	#endif

	state_lis3dh.accel_x = x;
	state_lis3dh.accel_y = y;
	state_lis3dh.accel_z = z;
}






