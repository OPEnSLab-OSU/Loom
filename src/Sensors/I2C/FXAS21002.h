///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_FXAS21002.h
/// @brief		File for Loom_FXAS21002 definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "I2C_Sensor.h"

#include <Adafruit_FXAS21002C.h>


///////////////////////////////////////////////////////////////////////////////
///
/// FXAS21002 3-Axis Gyroscope sensor module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___f_x_a_s21002.html)
/// - [Product Page: Adafruit Precision NXP 9-DOF Breakout Board - FXOS8700 + FXAS21002](https://www.adafruit.com/product/3463)
/// - [Dependency: Adafruit_FXAS21002C](https://github.com/adafruit/Adafruit_FXAS21002C)
/// - [Dependency: Adafruit_AHRS](https://github.com/adafruit/Adafruit_AHRS)
/// - [Dependency: Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
/// - [Datasheet: 3-Axis Digital Angular Rate Gyroscope](https://cdn-learn.adafruit.com/assets/assets/000/040/671/original/FXAS21002.pdf?1491475056)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#fxas21002-3-axis-gyroscope)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_FXAS21002 : public LoomI2CSensor
{

protected:
	
	Adafruit_FXAS21002C		inst_FXAS21002;		///< Underlying FXAS21002 sensor manager instance

	float					gyro[3];			///< Measured gyro values (x,y,z). Units: °/s

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	i2c_address				Set(Int) | <0x20> | {0x20, 0x21} | I2C address
	/// @param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	Loom_FXAS21002(
LoomManager* manager,
const byte i2c_address		= 0x20,
			const uint8_t		mux_port		= 255
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_FXAS21002(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_FXAS21002() = default;

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


