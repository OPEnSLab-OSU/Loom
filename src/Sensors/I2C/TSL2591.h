///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_TSL2591.h
/// @brief		File for Loom_TSL2591 definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "I2C_Sensor.h"

#include <Adafruit_TSL2591.h>


///////////////////////////////////////////////////////////////////////////////
///
/// TSL2591 Lux sensor module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___t_s_l2591.html)
/// - [Product Page: Adafruit TSL2591 High Dynamic Range Digital Light Sensor](https://www.adafruit.com/product/1980)
/// - [Dependency: Adafruit_TSL2591_Library](https://github.com/adafruit/Adafruit_TSL2591_Library)
/// - [Dependency: Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
/// - [Datasheet: TSL2591](https://cdn-shop.adafruit.com/datasheets/TSL25911_Datasheet_EN_v1.pdf)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#tsl2591-light-sensor)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_TSL2591 : public LoomI2CSensor
{

protected:

	Adafruit_TSL2591	inst_tsl2591;		///< Underlying TSL2591 sensor manager instance

	uint16_t			vis;				///< Measured visable. Units: lux.
	uint16_t			ir;					///< Measured infra-red. Units: lux.
	uint16_t			full;				///< Measured full spectrum. Units: lux.

	uint8_t				gain_level;			///< Sensor gain level setting to use
	uint8_t				timing_level;		///< Sensor integration time setting

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	i2c_address			Set(Int) | <0x29> | {0x29} | I2C address
	/// @param[in]	mux_port			Int | <255> | [0-16] | Port on multiplexer
	/// @param[in]	gain_level			Set(Int) | <1> | { 1 } | Gain level
	/// @param[in]	timing_level		Set(Int) | <0> | { 0 } | Timing level
	Loom_TSL2591(
LoomManager* manager,
const byte i2c_address			= 0x29,
			const uint8_t		mux_port			= 255,
			const uint8_t		gain_level			= 1,
			const uint8_t		timing_level		= 0
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_TSL2591(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_TSL2591() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_measurements() const override;

private:

};


