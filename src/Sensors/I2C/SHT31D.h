///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_SHT31D.h
/// @brief		File for Loom_SHT31D definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "I2C_Sensor.h"

#include <Adafruit_SHT31.h>


///////////////////////////////////////////////////////////////////////////////
///
/// SHT31D Temperature / Humidty sensor module
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___s_h_t31_d.html)
/// - [Product Page: Adafruit Sensirion SHT31-D - Temperature & Humidity Sensor](https://www.adafruit.com/product/2857)
/// - [Dependency: Adafruit_SHT31](https://github.com/adafruit/Adafruit_SHT31)
/// - [Datasheet: Datasheet SHT3x-DIS](https://cdn-shop.adafruit.com/product-files/2857/Sensirion_Humidity_SHT3x_Datasheet_digital-767294.pdf)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#sht31-d-temperature--humidity)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_SHT31D : public LoomI2CSensor
{

protected:
	
	Adafruit_SHT31	inst_sht31d;	///< Underlying SHT31D sensor manager instance

	float			temp;			///< Measured temperature. Units: C.
	float			humid;			///< Measured humidity Units: %.

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	i2c_address			Set(Int) | <0x44> | {0x44, 0x45} | I2C address
	/// @param[in]	mux_port			Int | <255> | [0-16] | Port on multiplexer
	Loom_SHT31D(
LoomManager* manager,
const byte i2c_address		= 0x44,
			const uint8_t		mux_port		= 255
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_SHT31D( LoomManager* manager, JsonArrayConst p );

	/// Destructor
	~Loom_SHT31D() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_measurements() const override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the temperature reading.
	/// Used by the LoomTempSync module to provide temperature
	/// to other modules.
	/// @return	The measured temperature
	float		get_temperature() const { return temp; }
	float		get_humidity() const { return humid; }
	
private:

};


