///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_TSL2561.h
/// @brief		File for Loom_TSL2561 definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "I2C_Sensor.h"

#include <Adafruit_TSL2561_U.h>


///////////////////////////////////////////////////////////////////////////////
///
/// TSL2561 Luminosity sensor module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___t_s_l2561.html)
/// - [Product Page: Adafruit TSL2561 Digital Luminosity/Lux/Light Sensor Breakout](https://www.adafruit.com/product/439)
/// - [Dependency: Adafruit_TSL2561](https://github.com/adafruit/Adafruit_TSL2561)
/// - [Dependency: Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
/// - [Datasheet: Light-To-Digital Converter](https://cdn-shop.adafruit.com/datasheets/TSL2561.pdf)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_TSL2561 : public LoomI2CSensor
{

protected:

	Adafruit_TSL2561_Unified	inst_TSL2561;	///< Underlying TSL2561 sensor manager instance

	uint8_t 	gain;			///< Gain level
	uint8_t 	resolution;		///< Sensor resolution setting

	uint16_t	lightIR;		///< Measured infra-red. Units: lux.
	uint16_t	lightFull;		///< Measure full spectrum. Units: lux.

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	i2c_address			Set(Int) | <0x39> | {0x29, 0x39, 0x49} | I2C address
	/// @param[in]	mux_port			Int | <255> | [0-16] | Port on multiplexer
	/// @param[in]	gain				Set(Int) | <1> | {1, 16} | Gain level
	/// @param[in]	resolution			Set(Int) | <3> | { 1("Low"), 2("Med"), 3("High") } | Resolution
	Loom_TSL2561(
LoomManager* manager,
const byte i2c_address		= 0x39,
			const uint8_t		mux_port		= 255,
			const uint8_t		gain			= 1,
			const uint8_t		resolution		= 3
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_TSL2561(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_TSL2561() = default;

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


