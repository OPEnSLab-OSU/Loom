///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_AS7263.h
/// @brief		File for Loom_AS7263 definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "I2C_Sensor.h"

#include <AS726X.h>


///////////////////////////////////////////////////////////////////////////////
///
/// AS7263 Near Infrared spectrum spectral sensor module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___a_s7263.html)
/// - [Product Page: SparkFun Spectral Sensor Breakout - AS7263 NIR](https://www.sparkfun.com/products/14351)
/// - [Dependency: Sparkfun AS726X Arduino Library](https://github.com/sparkfun/Qwiic_Spectral_Sensor_AS726X/tree/master/Libraries/Arduino)
/// - [Hookup Guide: AS726X NIR/VIS Spectral Sensor](https://learn.sparkfun.com/tutorials/as726x-nirvi)
/// - [Hardware GitHub](https://github.com/sparkfun/Qwiic_Spectral_Sensor_AS726X)
/// - [Datasheet: AS7263](https://cdn.sparkfun.com/assets/1/b/7/3/b/AS7263.pdf)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_AS7263 : public LoomI2CSensor
{

protected:

	AS726X		inst_AS7263;		///< Underlying AS7263 sensor manager instance

	uint16_t	nir_vals[6];		///< Measured near-infra-red band values (r,s,t,u,v,w).
									///< Units: counts / (μW/cm^2).

	
	bool		use_bulb;			///< Whether or not to use bulb
	uint8_t		gain;				///< Gain setting
	uint8_t		mode;				///< Sensor read mode
	uint8_t		integration_time;	///< Integration time setting

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	i2c_address					Set(Int) | <0x49> | {0x49} | I2C address
	/// @param[in]	mux_port					Int | <255> | [0-16] | Port on multiplexer
	/// @param[in]	use_bulb					Bool | <false> | {true, false} | Whether or not to use bulb
	/// @param[in]	gain						Set(Int) | <1> | { 0("1x"), 1("3.7x"), 2("16x"), 3("64x") } | Gain level
	/// @param[in]	mode						Set(Int) | <3> | { 0("Continuous reading of STUV"), 1("Continuous reading of RTUX"), 2("Continuous reading all channels"), 3("One-shot reading of all channels") } | Read mode
	/// @param[in]	integration_time			Int | <50> | [0-255] | Integration time (time will be 2.8ms * [integration value])
	Loom_AS7263(
LoomManager* manager,
const byte i2c_address			= 0x49,
			const uint8_t		mux_port			= 255,
			const bool			use_bulb			= false,
			const uint8_t		gain				= 1,
			const uint8_t		mode				= 3,
			const uint8_t		integration_time	= 50
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_AS7263(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_AS7263() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;
	void		print_measurements() const override;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set whether not bulb is used for active light source
	/// @param[in]	enable	Whether or not to enable bulb
	void		enable_bulb(const bool enable) { use_bulb = enable; }

	/// Set gain.
	/// @param[in]	gain	Gain level: 0: 1x (power-on default), 1: 3.7x, 2: 16x, 3: 64x
	void		set_gain(const uint8_t gain) { inst_AS7263.setGain(gain); }

	/// Set mode.
	/// @param[in]	mode	Mode
	///		0: Continuous reading of STUV
	///		1: Continuous reading of RTUX
	///		2: Continuous reading of all channels (power-on default)
	///		3: One-shot reading of all channels
	void		set_mode(const uint8_t mode) { inst_AS7263.setMeasurementMode(mode); }

	/// Set integration time.
	/// Time will be 2.8ms * [integration value]  (0-255), 50 is default
	/// @param[in]	time	Integration time
	void		set_integration_time(const uint8_t time) { inst_AS7263.setIntegrationTime(time); }

private:

};


