///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_TMP007.h
/// @brief		File for Loom_TMP007 definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "I2C_Sensor.h"

#include <Adafruit_TMP007.h>


///////////////////////////////////////////////////////////////////////////////
///
/// TMP007 thermopile temperature sensor module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___t_m_p007.html)
/// - [Product Page: Contact-less Infrared Thermopile Sensor Breakout - TMP007](https://www.adafruit.com/product/2023)
/// - [Dependency: Adafruit_TMP007_Library](https://github.com/adafruit/Adafruit_TMP007_Library)
/// - [DataSheet](https://cdn-shop.adafruit.com/datasheets/tmp007.pdf)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#tmp007-contact-less-infrared-thermopile-sensor)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_TMP007 : public LoomI2CSensor
{

protected:

	Adafruit_TMP007 inst_tmp007;	///< Underlying TMP007 sensor manager instance

	float		object_temp;		///< Object temperature. Units: °C.
	float		die_temp;			///< Die temperature. Units: °C.

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	i2c_address			Set(Int) | <0x40> | {0x40, 0x41, 0x42, 0x43, 0x46, 0x47, 0x48} | I2C address
	/// @param[in]	mux_port			Int | <255> | [0-16] | Port on multiplexer
	Loom_TMP007(
LoomManager* manager,
const byte i2c_address		= 0x40,
			const uint8_t	mux_port		= 255
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_TMP007(LoomManager* manager, JsonArrayConst p);
	
	/// Destructor
	virtual ~Loom_TMP007() = default;

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




