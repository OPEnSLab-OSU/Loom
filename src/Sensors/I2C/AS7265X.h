///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_AS7265X.h
/// @brief		File for Loom_AS7265X definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "I2C_Sensor.h"

#include <SparkFun_AS7265X.h>


///////////////////////////////////////////////////////////////////////////////
///
/// AS72625X 3 Spectrum (Visible, near IR, UV) spectral sensor module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___a_s7265_x.html)
/// - [Product Page: SparkFun Triad Spectroscopy Sensor - AS7265x](https://www.sparkfun.com/products/15050)
/// - [Dependency: SparkFun_AS7265x_Arduino_Library](https://github.com/sparkfun/SparkFun_AS7265x_Arduino_Library)
/// - [Datasheet: AS72625X](https://cdn.sparkfun.com/assets/c/2/9/0/a/AS7265x_Datasheet.pdf)
/// - [Hookup Guide: Spectral Triad (AS7265x)](https://learn.sparkfun.com/tutorials/spectral-triad-as7265x-hookup-guide)
/// - [Hardware GitHub](https://github.com/sparkfun/Qwiic_Spectral_Sensor_AS7265x)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#as7265x-spectral-sensor-triad-visible-near-infrared-ultraviolet)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_AS7265X : public LoomI2CSensor
{

protected:

	AS7265X		inst_AS7265X;		///< Underlying AS7265X sensor manager instance

	uint16_t	uv[6];				///< Measured UV bands values. Units: counts / (μW/cm^2)
	uint16_t	color[6];			///< Measured color bands values. Units: counts / (μW/cm^2)
	uint16_t	nir[6];				///< Measured near-infra-red bands values. Units: counts / (μW/cm^2)

	bool		use_bulb;			///< Whether or not to use the bulb

	uint8_t		gain;				///< Gain setting
	uint8_t		mode;				///< Sensor mode
	uint8_t		integration_time;	///< Integration time setting

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	i2c_address				Set(Int) | <0x49> | {0x49} | I2C address
	/// @param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	/// @param[in]	use_bulb				Bool | <false> | {true, false} | Whether or not to use bulb
	/// @param[in]	gain					Set(Int) | <1> | { 0("1x"), 1("3.7x"), 2("16x"), 3("64x") } | Gain level
	/// @param[in]	mode					Set(Int) | <3> | { 0("4 channels out of 6"), 1("Different 4 channels out of 6"), 2("All 6 channels continuously"), 3("One-shot reading of all channels") } | Read mode
	/// @param[in]	integration_time		Int | <50> | [0-255] | Integration time (time will be 2.8ms * [integration value])
	Loom_AS7265X(
LoomManager* manager,
const byte i2c_address			= 0x49,
			const uint8_t		mux_port			= 255,
			const bool			use_bulb			= false,
			const uint8_t		gain				= 64,
			const uint8_t		mode				= AS7265X_MEASUREMENT_MODE_6CHAN_ONE_SHOT,	// 4 channel, other 4 channel, 6 chan, or 6 chan one shot // AS7265X_Mode,
			const uint8_t		integration_time	= 50
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_AS7265X(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_AS7265X() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_measurements() const override;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set whether not bulb is used for active light source
	/// @param[in]	enable	Whether or not to enable 
	void		enable_bulb(const bool e) { use_bulb = e; }

	/// Set gain.
	/// 0: 1x (power-on default), 1: 3.7x, 2: 16x, 3: 64x
	/// @param[in]	gain	Gain level: 
	void		set_gain(const uint8_t gain) { inst_AS7265X.setGain(gain); }

	/// Set mode.
	/// 4 channel, other 4 channel, 6 chan, or 6 chan one shot.
	/// @param[in]	mode	Mode
	void		set_mode(const uint8_t mode) { inst_AS7265X.setMeasurementMode(mode); }

	/// Set integration time.
	/// 50 * 2.8ms = 140ms. 0 to 255 is valid.  (49 is default)
	/// If you use Mode 2 or 3 (all the colors) then integration time is double. 140*2 = 280ms between readings.
	void		set_integration_time(const uint8_t time) { inst_AS7265X.setIntegrationCycles(time); }

private:

};


