
#ifndef LOOM_FXAS21002_h
#define LOOM_FXAS21002_h


#include "Loom_I2C_Sensor.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXAS21002C.h>


class Loom_FXAS21002 : public LoomI2CSensor
{

protected:

	Adafruit_FXAS21002C* inst_FXAS21002;

	float gyro[3];

public:

	// --- CONSTRUCTOR ---
	Loom_FXAS21002(	byte i2c_address 			= 0x20, // 0x20 / 0x21
					
					char* module_name 			= "FXAS21002",
					char* sensor_description 	= "3-Axis Gyroscope" 
				);

	// --- DESTRUCTOR ---
	virtual ~Loom_FXAS21002();

	void print_measurements();

	void measure();
	void package(OSCBundle* bndl);
	void package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port);

private:

};


#endif