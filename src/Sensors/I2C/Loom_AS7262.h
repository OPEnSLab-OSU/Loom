///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_AS7262.h
/// @brief		File for Loom_AS7262 definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Loom_I2C_Sensor.h"

#include "AS726X.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// AS7262 Visible spectrum spectral sensor module
// ###
class Loom_AS7262 : public LoomI2CSensor
{

protected:

	AS726X		inst_AS7262;		///< Underlying AS7262 sensor manager instance

	uint16_t	color_vals[6];		///< Measured color  band values (violet, blue, green, yellow, orange, red)

	bool		use_bulb;			///< Whether or not to use bulb
	uint8_t		gain;				///< Gain setting
	uint8_t		mode;				///< Sensor read mode
	uint8_t		integration_time;	///< Integration time setting

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// AS7262 module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x49> | {0x49} | I2C address
	/// \param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	/// \param[in]	use_bulb				Bool | <false> | {true, false} | Whether or not to use bulb
	/// \param[in]	gain					Set(Int) | <1> | { 0("1x"), 1("3.7x"), 2("16x"), 3("64x") } | Gain level
	/// \param[in]	mode					Set(Int) | <3> | { 0("Continuous reading of VBGY"), 1("Continuous reading of GYOR"), 2("Continuous reading all channels"), 3("One-shot reading of all channels") } | Read mode
	/// \param[in]	integration_time		Int | <50> | [0-255] | Integration time (time will be 2.8ms * [integration value])
	Loom_AS7262(
			byte			i2c_address			= 0x49,
			uint8_t			mux_port			= 255,
			bool			use_bulb			= false,
			uint8_t			gain				= 1,
			uint8_t			mode				= 3,
			uint8_t			integration_time	= 50
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_AS7262(JsonArrayConst p);

	/// Destructor
	~Loom_AS7262() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_measurements() override;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set whether not bulb is used for active light source
	/// \param[in]	enable	Whether or not to enable 
	void		enable_bulb(bool enable) { use_bulb = enable; }

	/// Set gain.
	/// \param[in]	gain	Gain level: 0: 1x (power-on default), 1: 3.7x, 2: 16x, 3: 64x
	void		set_gain(uint8_t gain) { inst_AS7262.setGain(gain); }

	/// Set mode.
	/// \param[in]	mode	Mode.
	///		0: Continuous reading of VBGY
	///		1: Continuous reading of GYOR
	///		2: Continuous reading of all channels (power-on default)
	///		3: One-shot reading of all channels
	void		set_mode(uint8_t mode) { inst_AS7262.setMeasurementMode(mode); }

	/// Set integration time.
	/// Time will be 2.8ms * [integration value]  (0-255), 50 is default
	/// \param[in]	time	Integration time
	void		set_integration_time(uint8_t time) { inst_AS7262.setIntegrationTime(time); }

private:

};

