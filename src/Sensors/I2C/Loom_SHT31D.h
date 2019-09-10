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

#include "Loom_I2C_Sensor.h"

#include "Adafruit_SHT31.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// SHT31D Temperature / Humidty sensor module
// ###
class Loom_SHT31D : public LoomI2CSensor
{

protected:
	
	Adafruit_SHT31	inst_sht31d;	///< Underlying SHT31D sensor manager instance

	float			temp;			///< Measured temperature (C)
	float			humid;			///< Measured humidity (%)

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// SHT31D module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x44> | {0x44, 0x45} | I2C address
	/// \param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	Loom_SHT31D(
			byte			i2c_address		= 0x44,
			uint8_t			mux_port		= 255
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_SHT31D( JsonArrayConst p );

	/// Destructor
	~Loom_SHT31D() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_measurements() override;

private:

};


