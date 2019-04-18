
#ifndef LOOM_AS7265X_h
#define LOOM_AS7265X_h

#include "Loom_I2C_Sensor.h"

#include "SparkFun_AS7265X.h"


// # (LoomI2CSensor) | dependencies: [] | conflicts: []
/// AS72625X 3 Spectrum (Visible, near IR, UV) spectral sensor module 
// #
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


	/// AS72625X module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x49> | {0x49} | I2C address
	/// \param[in]	module_name				String | <"AS72625X"> | AS72625X module name					
	/// \param[in]	use_bulb				Bool | <false> | Whether or not to use bulb
	/// \param[in]	gain					Set(Int) | <1> | { 0("1x"), 1("3.7x"), 2("16x"), 3("64x") } | Gain level
	/// \param[in]	mode					Set(Int) | <3> | { 0("4 channels out of 6"), 1("Different 4 channels out of 6"), 2("All 6 channels continuously"), 3("One-shot reading of all channels") } | Read mode
	/// \param[in]	integration_time		Int | <50> | [0-255] | Integration time (time will be 2.8ms * [integration value]) 
	Loom_AS7265X(	
			byte		i2c_address				= 0x49,
			char*		module_name				= "AS7265X",
			bool		use_bulb				= false,
			byte		gain					= 64,
			byte		mode					= AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT,	// 4 channel, other 4 channel, 6 chan, or 6 chan one shot // AS7265X_Mode,
			byte		integration_time		= 50
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
