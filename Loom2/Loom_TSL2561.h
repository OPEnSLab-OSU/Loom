
#ifndef LOOM_TSL2561_h
#define LOOM_TSL2561_h


#include "Loom_I2C_Sensor.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>



class Loom_TSL2561 : public LoomI2CSensor
{

protected:

	Adafruit_TSL2561_Unified* inst_TSL2561;

	int gain;
	int resolution;

	int lightIR;
	int lightFull;

public:

	// --- CONSTRUCTOR ---
	Loom_TSL2561(	byte i2c_address 			= 0x29, // 0x29, 0x39, 0x49
					
					char* module_name 			= "TSL2561",
					char* sensor_description 	= "Luminosity",

					int gain 					= 1, // 1, 16
					int resolution 				= 3	 // 1, 2, 3
				);

	// --- DESTRUCTOR ---
	virtual ~Loom_TSL2561();

	void print_measurements();

	void measure();
	void package(OSCBundle* bndl, char* suffix="");
	// void package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port);

private:

};


#endif