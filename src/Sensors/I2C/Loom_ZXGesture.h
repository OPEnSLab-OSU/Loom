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

#include "Loom_I2C_Sensor.h"

#include <ZX_Sensor.h>


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// ZXGesture position / gesture sensor module
// ###
class Loom_ZXGesture : public LoomI2CSensor
{

public:

	/// Different modes of the ZX gesture sensor
	enum class Mode {
		POS,	///< Measure z and x position
		GEST 	///< Measure gestures
	};

protected:

	/// Underlying ZX sensor manager instance
	ZX_Sensor		inst_ZX;

	/// Sensor mode (read positions or gesture - mutually exclusive)
	Mode			mode;

	/// Measured positions (x, y pos)
	uint8_t			pos[2];

	/// Last read gesture type
	GestureType		gesture;

	/// String labelling last read gesture type
	String			gesture_type;
	
	/// The speed of the last measured gesture
	uint8_t			gesture_speed;

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// ZXGesture module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x10> | {0x10, 0x11} | I2C address
	/// \param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	/// \param[in]	mode					Set(ZXMode) | <0> | { 0("Position"), 1("Gesture") } | Gain level
	Loom_ZXGesture(
			byte			i2c_address		= 0x10,
			uint8_t			mux_port		= 255,
			Mode			mode			= Mode::POS
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_ZXGesture(JsonArrayConst p);

	/// Destructor
	~Loom_ZXGesture() = default;

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

private:

};


