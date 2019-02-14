
#ifndef LOOM_FXOS8700_h
#define LOOM_FXOS8700_h


#include "Loom_I2C_Sensor.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_FXOS8700.h>


class Loom_FXOS8700 : public LoomI2CSensor
{

protected:

	Adafruit_FXOS8700* inst_FXOS8700;

	float accel[3];
	float mag[3];

public:

	// --- CONSTRUCTOR ---
	Loom_FXOS8700(	byte i2c_address 			= 0x1F, // 0x1C, 0x1D, 0x1E, 0x1F
					
					char* module_name 			= "FXOS8700",
					char* sensor_description 	= "3-Axis Acceleromter/Magnetometer" 
				);

	// --- DESTRUCTOR ---
	virtual ~Loom_FXOS8700();

	void print_measurements();

	void measure();
	void package(OSCBundle* bndl, char* suffix="");

private:

};


#endif