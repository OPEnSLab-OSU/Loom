
#ifndef LOOM_AS7265X_h
#define LOOM_AS7265X_h


#include "Loom_I2C_Sensor.h"

#include "SparkFun_AS7265X.h"



class Loom_AS7265X : public LoomI2CSensor
{

protected:

	AS7265X inst_AS7265X;

	int[6] uv;
	int[6] color;
	int[6] nir;

	bool use_bulb;

public:

	// --- CONSTRUCTOR ---
	Loom_AS7265X(	byte i2c_address 			= 0x49,
					
					char* module_name 			= "AS7265X",
					char* sensor_description 	= "Spectral Triad",

					bool use_bulb 				= false	
				);

	// --- DESTRUCTOR ---
	~Loom_AS7265X();

	void print_measurements();

	void measure();
	void package(OSCBundle* bndl);
	void package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port);

	void use_bulb(bool e);

private:

};


#endif