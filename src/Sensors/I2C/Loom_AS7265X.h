#pragma once

#include "Loom_I2C_Sensor.h"

#include "SparkFun_AS7265X.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// AS72625X 3 Spectrum (Visible, near IR, UV) spectral sensor module
// ###
class Loom_AS7265X : public LoomI2CSensor
{

protected:

	AS7265X		inst_AS7265X;		/// Underlying AS7265X sensor manager instance

	int			uv[6];				/// Measured UV bands values
	int			color[6];			/// Measured color bands values
	int			nir[6];				/// Measured near-infra-red bands values

	bool		use_bulb;			/// Whether or not to use the bulb

	byte		gain;				/// Gain setting
	byte		mode;				/// Sensor mode
	byte		integration_time;	/// Integration time setting


public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// AS72625X module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x49> | {0x49} | I2C address
	/// \param[in]	module_name				String | <"AS72625X"> | AS72625X module name
	/// \param[in]	use_bulb				Bool | <false> | {true, false} | Whether or not to use bulb
	/// \param[in]	gain					Set(Int) | <1> | { 0("1x"), 1("3.7x"), 2("16x"), 3("64x") } | Gain level
	/// \param[in]	mode					Set(Int) | <3> | { 0("4 channels out of 6"), 1("Different 4 channels out of 6"), 2("All 6 channels continuously"), 3("One-shot reading of all channels") } | Read mode
	/// \param[in]	integration_time		Int | <50> | [0-255] | Integration time (time will be 2.8ms * [integration value])
	Loom_AS7265X(
			byte			i2c_address			= 0x49,
			const char*		module_name			= "AS7265X",
			bool			use_bulb			= false,
			byte			gain				= 64,
			byte			mode				= AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT,	// 4 channel, other 4 channel, 6 chan, or 6 chan one shot // AS7265X_Mode,
			byte			integration_time	= 50
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_AS7265X(JsonVariant p);

	/// Destructor
	~Loom_AS7265X() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_measurements() override;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set whether not bulb is used for active light source
	/// \param[in]	enable	Whether or not to enable 
	void		enable_bulb(bool e);

	/// Set gain.
	// 0: 1x (power-on default), 1: 3.7x, 2: 16x, 3: 64x
	/// \param[in]	gain	Gain level: 
	void		set_gain(byte gain);

	/// Set mode.
	/// 4 channel, other 4 channel, 6 chan, or 6 chan one shot
	/// \param[in]	mode	Mode
	void		set_mode(byte mode);

	/// Set integration time.
	/// 50 * 2.8ms = 140ms. 0 to 255 is valid.  (49 is default)
	/// If you use Mode 2 or 3 (all the colors) then integration time is double. 140*2 = 280ms between readings.
	void		set_integration_time(byte time);

private:

};


