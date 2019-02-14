
#ifndef LOOM_AS7263_h
#define LOOM_AS7263_h


#include "Loom_I2C_Sensor.h"

#include "AS726X.h"


class Loom_AS7263 : public LoomI2CSensor
{

protected:

	AS726X inst_AS7263;

	int nir_r, nir_s, nir_t, nir_u, nir_v, nir_w;

	bool use_bulb;
	byte gain;
	byte mode;
	byte integration_time;

public:

	// --- CONSTRUCTOR ---
	Loom_AS7263(	byte i2c_address 			= 0x49,
					
					char* module_name 			= "AS7263",
					char* sensor_description 	= "Spectral Sensor (near ir)",

					bool use_bulb 				= false,
					byte gain 					= 1, 	// 0: 1x (power-on default), 1: 3.7x, 2: 16x, 3: 64x
					byte mode 					= 3, 	// 0: Continuous reading of STUV 
														// 1: Continuous reading of RTUX 
														// 2: Continuous reading of all channels (power-on default)
														// 3: One-shot reading of all channels
					byte integration_time		= 50  	// Time will be 2.8ms * [integration value]  (0-255), 50 is default
				);

	// --- DESTRUCTOR ---
	virtual ~Loom_AS7263();

	void print_config();
	void print_measurements();

	void measure();
	void package(OSCBundle* bndl, char* suffix="");

	void enable_bulb(bool enable);
	void set_gain(byte gain);
	void set_mode(byte mode);
	void set_integration_time(byte time);

private:

};


#endif