///////////////////////////////////////////////////////////////////////////////
///
/// @file		FXOS8700.h
/// @brief		File for FXOS8700 definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "I2C_Sensor.h"

#include <Adafruit_FXOS8700.h>

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// FXOS8700 3-Axis Acceleromter/Magnetometer sensor module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___f_x_o_s8700.html)
/// - [Product Page: Adafruit Precision NXP 9-DOF Breakout Board - FXOS8700 + FXAS21002](https://www.adafruit.com/product/3463)
/// - [Dependency: Adafruit_FXOS8700](https://github.com/adafruit/Adafruit_FXOS8700)
/// - [Dependency: Adafruit_AHRS](https://github.com/adafruit/Adafruit_AHRS)
/// - [Dependency: Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
/// - [Datasheet: FXOS8700CQ](https://cdn-learn.adafruit.com/assets/assets/000/043/458/original/FXOS8700CQ.pdf?1499125614)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#fxos8700-3-axis-accelerometermagentometer)
///
///////////////////////////////////////////////////////////////////////////////
class FXOS8700 : public I2CSensor
{
protected:

	Adafruit_FXOS8700	inst_FXOS8700;		///< Underlying FXOS8700 sensor manager instance

	float		accel[3];					///< Measured acceleration values (x,y,z). Units: g
	float		mag[3];						///< Measured magnetic field values (x,y,z). Units: μT

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	i2c_address				Set(Int) | <0x1F> | {0x1C, 0x1D, 0x1E, 0x1F} | I2C address
	/// @param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	FXOS8700(
			const byte		i2c_address		= 0x1F,
			const uint8_t	mux_port		= 255
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	FXOS8700(JsonArrayConst p);

	/// Destructor
	~FXOS8700() = default;

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

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, FXOS8700, "FXOS8700");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS
