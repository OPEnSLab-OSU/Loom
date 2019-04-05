
#ifndef LOOM_AS7262_h
#define LOOM_AS7262_h

#include "Loom_I2C_Sensor.h"

#include "AS726X.h"


/// AS7262 Visible spectrum spectral sensor module 
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
			byte		i2c_address				=  0x49, // AS7262_I2C_Address,
			
			char*		module_name				=  "AS7262", // AS7262_Module_Name,
			char*		sensor_description		=  "Spectral Sensor (visible)", // AS7262_Sensor_Description,

			bool		use_bulb				=  false, //AS7262_Use_Bulb,
			byte		gain					=  1, // AS7262_Gain	// 0: 1x (power-on default), 1: 3.7x, 2: 16x, 3: 64x
			byte		mode					=  3, // AS7262_Mode	// 0: Continuous reading of VBGY 
												// 1: Continuous reading of GYOR 
												// 2: Continuous reading of all channels (power-on default)
												// 3: One-shot reading of all channels
			byte		integration_time		= 50 //  AS7262_Integration_Time   	// Time will be 2.8ms * [integration value]  (0-255), 50 is default
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_AS7262();

	// Inherited (Overriding) Methods
	void		print_config() override;
	void		print_measurements() override;
	void		measure() override;
	void		package(OSCBundle& bndl, char* suffix="") override;

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
