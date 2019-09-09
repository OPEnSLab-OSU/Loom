#pragma once

#include "Loom_I2C_Sensor.h"

// #include <MS5803_01.h>
#include <MS5803_02.h>


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// MS5803 Atmospheric Pressure / Temperature sensor module
// ###
class Loom_MS5803 : public LoomI2CSensor
{

protected:

	MS_5803		inst_MS5803;	///< Underlying MS5803 sensor manager instance

	uint16_t	MS_PROM[8];		///< Read calibration data stored on sensor

	float		pressure;		///< Measure pressure (mbar)
	float		temp;			///< Measured temperature (C)

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// MS5803 module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x76> | {0x76, 0x77} | I2C address (0x76 if CSB (pin 3) is High, 0x77 if CSB is Low)
	/// \param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	Loom_MS5803(
			byte			i2c_address		= 0x76,
			uint8_t			mux_port		= 255
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_MS5803(JsonArrayConst p);

	/// Destructor
	~Loom_MS5803() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_measurements() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the temperature reading.
	/// Used by the LoomTempSync module to provide temperature
	/// to other modules.
	/// \return	The measured temperature
	float		get_temperature() { return temp; }

private:

};



