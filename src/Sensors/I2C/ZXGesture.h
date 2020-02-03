///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_ZXGesture.h
/// @brief		File for Loom_ZXGesture definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "I2C_Sensor.h"

#include <ZX_Sensor.h>


///////////////////////////////////////////////////////////////////////////////
///
/// ZXGesture position / gesture sensor module.
///
/// @note	Only position or gestures can be measured at a given time, not both.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___z_x_gesture.html)
/// - [Product Page: Sparkfun ZX Distance and Gesture Sensor](https://www.sparkfun.com/products/13162)
/// - [Dependency: SparkFun ZX Distance and Gesture Sensor Arduino Library](https://github.com/sparkfun/SparkFun_ZX_Distance_and_Gesture_Sensor_Arduino_Library)
/// - [Datasheet: ZX SparkFun Sensor Datasheet](https://cdn.sparkfun.com/assets/learn_tutorials/3/4/5/XYZ_Interactive_Technologies_-_ZX_SparkFun_Sensor_Datasheet.pdf)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_ZXGesture : public LoomI2CSensor
{

public:

	/// Different modes of the ZX gesture sensor
	enum class Mode {
		POS,	///< Measure z and x position
		GEST 	///< Measure gestures
	};

protected:

	ZX_Sensor		inst_ZX;		///< Underlying ZX sensor manager instance
	Mode			mode;			///< Sensor mode (read positions or gesture - mutually exclusive)
	uint8_t			pos[2];			///< Measured positions (x, y pos). Units: mm.
	GestureType		gesture;		///< Last read gesture type
	String			gesture_type;	///< String labelling last read gesture type
	uint8_t			gesture_speed;	///< The speed of the last measured gesture

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	i2c_address				Set(Int) | <0x10> | {0x10, 0x11} | I2C address
	/// @param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	/// @param[in]	mode					Set(ZXMode) | <0> | { 0("Position"), 1("Gesture") } | Gain level
	Loom_ZXGesture(
LoomManager* manager,
const byte i2c_address		= 0x10,
			const uint8_t		mux_port		= 255,
			const Mode			mode			= Mode::POS
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_ZXGesture(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_ZXGesture() = default;

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

private:

};


