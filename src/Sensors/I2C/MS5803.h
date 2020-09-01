///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_MS5803.h
/// @brief		File for Loom_MS5803 definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "I2C_Sensor.h"

#include <MS5803_02.h>


///////////////////////////////////////////////////////////////////////////////
///
/// MS5803 Atmospheric Pressure / Temperature sensor module.
///
/// @attention	Library is modified to support different I2C addresses and will not 
///				compile with Loom without the change.
///				The modifed verision can be found in the [Loom_Auxiliary](https://github.com/OPEnSLab-OSU/Loom_Auxiliary/blob/master/Dependencies/Dependencies_Individual/MS5803_02.zip) 
///				repository
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___m_s5803.html)
/// - [Product Page: MS580302BA01-00 Miniature Altimeter Module](https://www.digikey.com/product-detail/en/te-connectivity-measurement-specialties/MS580302BA01-00/223-1624-5-ND/5277629)
/// - [Dependency: MS5803_2](https://github.com/millerlp/MS5803_02)
/// - [Datasheet: MS5803-02BA](https://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=MS5803-02BA&DocType=Data+Sheet&DocLang=English)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#ms5803-atmospheric-pressure--temperature-sensor)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_MS5803 : public LoomI2CSensor
{

protected:

	MS_5803		inst_MS5803;	///< Underlying MS5803 sensor manager instance

	uint16_t	MS_PROM[8];		///< Read calibration data stored on sensor

	float		pressure;		///< Measure pressure. Units: mbar.
	float		temp;			///< Measured temperature. Units: °C.

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	i2c_address				Set(Int) | <0x76> | {0x76, 0x77} | I2C address (0x76 if CSB (pin 3) is High, 0x77 if CSB is Low)
	/// @param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	Loom_MS5803(
LoomManager* manager,
const byte i2c_address		= 0x76,
			const uint8_t		mux_port		= 255
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_MS5803(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_MS5803() = default;

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

private:

};



