
#ifndef LOOM_ZXGESTURE_h
#define LOOM_ZXGESTURE_h


#include "Loom_I2C_Sensor.h"

#include <Wire.h>
#include <ZX_Sensor.h>

enum ZXMode { ZX_POS, ZX_GEST };



class Loom_ZXGesture : public LoomI2CSensor
{

protected:

	ZX_Sensor* inst_ZX;

	ZXMode mode;

	int pos[2];

	GestureType gesture;
	String      gesture_type;
	uint8_t     gesture_speed;

public:

	// --- CONSTRUCTOR ---
	Loom_ZXGesture(	byte i2c_address 			= 0x10, // 0x10, 0x11
					
					char* module_name 			= "ZXGesture",
					char* sensor_description 	= "Temperature/Humidty",

					ZXMode mode 				= ZX_POS
				);

	// --- DESTRUCTOR ---
	~Loom_ZXGesture();

	void print_config();
	void print_measurements();

	void measure();
	void package(OSCBundle* bndl);
	void package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port);

private:

};


#endif