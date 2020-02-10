///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_LIS3DH.h
/// @brief		File for Loom_LIS3DH definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "I2C_Sensor.h"

#include <SparkFunLIS3DH.h>


///////////////////////////////////////////////////////////////////////////////
///
/// LIS3DH Accelerometer sensor module.
///
/// @note	Needs more work on properly implementing support for the sensors interrupts 
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___l_i_s3_d_h.html)
/// - [Product Page: SparkFun Triple Axis Accelerometer Breakout - LIS3DH](https://www.sparkfun.com/products/13963)
/// - [Dependency: Sparkfun LIS3DH Arduino Library](https://github.com/sparkfun/SparkFun_LIS3DH_Arduino_Library)
/// - [Hookup Guide: LIS3DH](https://learn.sparkfun.com/tutorials/lis3dh-hookup-guide)
/// - [Hardware GitHub](https://github.com/sparkfun/LIS3DH_Breakout)
/// - [Datasheet: LIS3DH](https://cdn.sparkfun.com/assets/f/b/c/c/f/AS7262.pdf)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_LIS3DH : public LoomI2CSensor
{

protected:

	LIS3DH		inst_LIS3DH;	///< Underlying LIS3DH sensor manager instance

	float		accel[3];		///< Measured acceleration values (x,y,z). Units: g.

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	i2c_address				Set(Int) | <0x19> | {0x19} | I2C address
	/// @param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	Loom_LIS3DH(
LoomManager* manager,
const byte i2c_address		= 0x19,
			const uint8_t	mux_port		= 255
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_LIS3DH(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_LIS3DH() = default;

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



