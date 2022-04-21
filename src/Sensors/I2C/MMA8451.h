///////////////////////////////////////////////////////////////////////////////
///
/// @file		MMA.h
/// @brief		File for Loom_MMA definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "I2C_Sensor.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_MMA8451.h>

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// MMA8451 accelerometer sensor module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___m_m_a8451.html)
/// - [Product Page: Adafruit Triple-Axis Accelerometer - ±2/4/8g @ 14-bit - MMA8451](https://www.adafruit.com/product/2019)
/// - [Dependency: Adafruit_MMA8451_Library](https://github.com/adafruit/Adafruit_MMA8451_Library)
/// - [Dependency: Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
/// - [Datasheet: Xtrinsic MMA8451Q 3-Axis, 14-bit/8-bit Digital Accelerometer](https://cdn-shop.adafruit.com/datasheets/MMA8451Q-1.pdf)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#mma8451-triple-axis-accelerometer)
///
///////////////////////////////////////////////////////////////////////////////
class MMA8451 : public I2CSensor
{
protected:

	Adafruit_MMA8451 MMA;			///< Underlying MMA8451 sensor manager instance

	float			accel[3];		///< Acceleration values for each axis. Units: g
	uint8_t			orientation;	///< Orientation

	mma8451_range_t	range;			///< Range setting (2/4/8 G)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	i2c_address			Set(Int) | <0x1D> | {0x1C, 0x1D} | I2C address
	/// @param[in]	mux_port			Int | <255> | [0-16] | Port on multiplexer
	/// @param[in]	range				Set() | <"MMA8451"> | null | MMA8451 module name
	MMA8451(
			const byte				i2c_address	= 0x1D,
			const uint8_t			mux_port	= 255,
			const mma8451_range_t	range		= MMA8451_RANGE_2_G
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	MMA8451(JsonArrayConst p);
	
	/// Destructor
	virtual ~MMA8451() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void		package(JsonObject json) override;

	// void		enable_interrupts(bool enable = true);
	// void 		set_transient_int_threshold(uint8_t range);

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;
	void		print_measurements() const override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================


//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	// Needs refinement
	// void 		configure_interrupts();

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================


private:

};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, MMA8451, "MMA8451");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS


