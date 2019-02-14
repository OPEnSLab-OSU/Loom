
#ifndef LOOM_AS7262_h
#define LOOM_AS7262_h


#include "Loom_I2C_Sensor.h"

#include "AS726X.h"



class Loom_AS7262 : public LoomI2CSensor
{

protected:

	AS726X inst_AS7262;

	int violet, blue, green, yellow, orange, red;

	bool use_bulb;
	byte gain;
	byte mode;
	byte integration_time;

public:

	// --- CONSTRUCTOR ---
	Loom_AS7262(	byte i2c_address 			= 0x49,
					
					char* module_name 			= "AS7262",
					char* sensor_description 	= "Spectral Sensor (visible)",

					bool use_bulb 				= false,
					byte gain 					= 1, 	// 0: 1x (power-on default), 1: 3.7x, 2: 16x, 3: 64x
					byte mode 					= 3, 	// 0: Continuous reading of VBGY 
														// 1: Continuous reading of GYOR 
														// 2: Continuous reading of all channels (power-on default)
														// 3: One-shot reading of all channels
					byte integration_time		= 50  	// Time will be 2.8ms * [integration value]  (0-255), 50 is default
				);


//Gain 

	// --- DESTRUCTOR ---
	virtual ~Loom_AS7262();

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