
#ifndef LOOM_AS7263_h
#define LOOM_AS7263_h


#include "Loom_I2C_Sensor.h"

#include "AS726X.h"


class Loom_AS7263 : public LoomI2CSensor
{

protected:

	AS726X inst_AS7263;

	int nir_r, nir_s, nir_t, nir_u, nir_v, nir_w;

	int gain;
	int mode;
	bool use_bulb;

public:

	// --- CONSTRUCTOR ---
	Loom_AS7263(	byte i2c_address 			= 0x49,
					
					char* module_name 			= "AS7263",
					char* sensor_description 	= "Spectral Sensor (near ir)" 

					bool use_bulb 				= false,
					int gain 					= 3,
					int mode 					= 3
				);

	// --- DESTRUCTOR ---
	~Loom_AS7263();

	void print_config();
	void print_measurements();

	void measure();
	void package(OSCBundle* bndl);
	void package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port);

	void use_bulb(bool e);

private:

};


#endif