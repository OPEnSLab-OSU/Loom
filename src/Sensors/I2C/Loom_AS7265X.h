
#ifndef LOOM_AS7265X_h
#define LOOM_AS7265X_h

#include "Loom_I2C_Sensor.h"

#include "SparkFun_AS7265X.h"



class Loom_AS7265X : public LoomI2CSensor
{

protected:

	/// Underlying AS7265X sensor manager instance
	AS7265X		inst_AS7265X;

	/// Measured UV bands values
	int			uv[6];
	/// Measured color bands values
	int			color[6];
	/// Measured near-infra-red bands values
	int			nir[6];

	/// Whether or not to use the bulb
	bool		use_bulb;

	/// Gain setting 
	byte		gain;
	/// Sensor mode
	byte		mode;
	/// Integration time setting
	byte		integration_time;


public:

	// --- CONSTRUCTOR ---
	Loom_AS7265X(	
			byte		i2c_address				= AS7265X_I2C_Address, // 0x49,
			
			char*		module_name				= AS7265X_Module_Name, // "AS7265X",
			char*		sensor_description		= AS7265X_Sensor_Description, // "Spectral Triad",

			bool		use_bulb				= AS7265X_Use_Bulb, // false,	
			byte		gain					= AS7265X_Gain, // 64,	// 1 to 64x
			byte		mode					= AS7265X_Mode, // AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT,	// 4 channel, other 4 channel, 6 chan, or 6 chan one shot
			byte		integration_time		= AS7265X_Integration_Time // 49 	// Time will be 2.8ms * [integration value]  (0-255), 50 is default
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_AS7265X();

	// Inherited (Overriding) Methods
	void		print_measurements();
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");


	void		enable_bulb(bool e);
	// 1 to 64x
	void		set_gain(byte gain);
	//4 channel, other 4 channel, 6 chan, or 6 chan one shot
	void		set_mode(byte mode);
	//50 * 2.8ms = 140ms. 0 to 255 is valid.  (49 is default)
	//If you use Mode 2 or 3 (all the colors) then integration time is double. 140*2 = 280ms between readings.
	void		set_integration_time(byte time);

private:

};


#endif
