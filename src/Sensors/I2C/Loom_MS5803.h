
#ifndef LOOM_MS5803_h
#define LOOM_MS5803_h

#include "Loom_I2C_Sensor.h"

#include <Wire.h>
#include <MS5803_02.h> 


class Loom_MS5803 : public LoomI2CSensor
{

protected:

	MS_5803* inst_MS5803;

	uint16_t MS_PROM[8]; //storing calibration data

	float temp;
	float pressure;

public:

	// --- CONSTRUCTOR ---
	Loom_MS5803(	byte i2c_address 			= 0x77, // 0x76 if CSB (pin 3) is High, 0x77 if CSB is Low
					
					char* module_name 			= "MS5803",
					char* sensor_description 	= "Atmospheric Pressure / Temperature" 
				);

	// --- DESTRUCTOR ---
	virtual ~Loom_MS5803();

	void print_measurements();

	void measure();
	void package(OSCBundle& bndl, char* suffix="");

private:

};


#endif
