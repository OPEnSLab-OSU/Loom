
#ifndef LOOM_AS7262_h
#define LOOM_AS7262_h


#include "Loom_I2C_Sensor.h"

#include "AS726X.h"


class Loom_AS7262 : public LoomI2CSensor
{

protected:

	AS726X inst_AS7262;

	int violet, blue, green, yellow, orange, red;

	int gain;
	int mode;
	bool use_bulb;

public:

	// --- CONSTRUCTOR ---
	Loom_AS7262(	byte i2c_address 			= 0x49,
					
					char* module_name 			= "AS7262",
					char* sensor_description 	= "Spectral Sensor (visible)",

					bool use_bulb 				= false,
					int gain 					= 3,
					int mode 					= 3
				);

	// --- DESTRUCTOR ---
	~Loom_AS7262();

	void print_config();
	void print_measurements();

	void measure();
	void package(OSCBundle* bndl);
	void package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port);

	void enable_bulb(bool e);

private:

};


#endif