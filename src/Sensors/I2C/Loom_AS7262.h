
#ifndef LOOM_AS7262_h
#define LOOM_AS7262_h

#include "Loom_I2C_Sensor.h"

#include "AS726X.h"



class Loom_AS7262 : public LoomI2CSensor
{

protected:

	/// Underlying AS7262 sensor manager instance
	AS726X		inst_AS7262;

	/// Measured color  band values (violet, blue, green, yellow, orange, red)
	int			color_vals[6];

	/// Whether or not to use bulb
	bool		use_bulb;

	/// Gain setting
	byte		gain;
	/// Sensor read mode
	byte		mode;
	/// Integration time setting
	byte		integration_time;

public:

	// --- CONSTRUCTOR ---
	Loom_AS7262(	
			byte		i2c_address				= 0x49,
			
			char*		module_name				= "AS7262",
			char*		sensor_description		= "Spectral Sensor (visible)",

			bool		use_bulb				= false,
			byte		gain					= 1,	// 0: 1x (power-on default), 1: 3.7x, 2: 16x, 3: 64x
			byte		mode					= 3, 	// 0: Continuous reading of VBGY 
												// 1: Continuous reading of GYOR 
												// 2: Continuous reading of all channels (power-on default)
												// 3: One-shot reading of all channels
			byte		integration_time		= 50  	// Time will be 2.8ms * [integration value]  (0-255), 50 is default
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_AS7262();

	// Inherited (Overriding) Methods
	void		print_config();
	void		print_measurements();
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");

	void		enable_bulb(bool enable);
	// 0: 1x (power-on default), 1: 3.7x, 2: 16x, 3: 64x
	void		set_gain(byte gain);
	// 0: Continuous reading of VBGY 
	// 1: Continuous reading of GYOR 
	// 2: Continuous reading of all channels (power-on default)
	// 3: One-shot reading of all channels
	void		set_mode(byte mode);
	// Time will be 2.8ms * [integration value]  (0-255), 50 is default
	void		set_integration_time(byte time);

private:

};


#endif
