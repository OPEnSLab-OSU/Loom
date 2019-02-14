
#ifndef LOOM_AS7265X_h
#define LOOM_AS7265X_h


#include "Loom_I2C_Sensor.h"

#include "SparkFun_AS7265X.h"



class Loom_AS7265X : public LoomI2CSensor
{

protected:

	AS7265X inst_AS7265X;

	int uv[6];
	int color[6];
	int nir[6];

	bool use_bulb;
	byte gain;
	byte mode;
	byte integration_time;


public:

	// --- CONSTRUCTOR ---
	Loom_AS7265X(	byte i2c_address 			= 0x49,
					
					char* module_name 			= "AS7265X",
					char* sensor_description 	= "Spectral Triad",

					bool use_bulb 				= false,	
					byte gain 					= 64, 	// 1 to 64x
					byte mode 					= AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT, 	//4 channel, other 4 channel, 6 chan, or 6 chan one shot
					byte integration_time		= 49  	// Time will be 2.8ms * [integration value]  (0-255), 50 is default
				);

	// --- DESTRUCTOR ---
	virtual ~Loom_AS7265X();

	void print_measurements();

	void measure();
	void package(OSCBundle* bndl, char* suffix="");

	void enable_bulb(bool e);
	void set_gain(byte gain);
	void set_mode(byte mode);
	void set_integration_time(byte time);

private:

};


#endif