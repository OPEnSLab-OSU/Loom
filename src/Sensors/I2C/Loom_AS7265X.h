
#ifndef LOOM_AS7265X_h
#define LOOM_AS7265X_h

#include "Loom_I2C_Sensor.h"

#include "SparkFun_AS7265X.h"


/// AS7262 3 Spectrum (Visible, near IR, UV) spectral sensor module 
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
			byte		i2c_address				=  0x49, // AS7265X_I2C_Address,
			
			char*		module_name				=  "AS7265X", // AS7265X_Module_Name,
			char*		sensor_description		=  "Spectral Triad", // AS7265X_Sensor_Description,

			bool		use_bulb				=  false,	 // AS7265X_Use_Bulb,
			byte		gain					=  64,	// 1 to 64x // AS7265X_Gain,
			byte		mode					=  AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT,	// 4 channel, other 4 channel, 6 chan, or 6 chan one shot // AS7265X_Mode,
			byte		integration_time		= 49 	// Time will be 2.8ms * [integration value]  (0-255), 50 is default // AS7265X_Integration_Time 
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_AS7265X();

	// Inherited (Overriding) Methods
	void		print_measurements() override;
	void		measure() override;
	void		package(OSCBundle& bndl, char* suffix="") override;


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
