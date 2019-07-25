#pragma once

#include "Loom_I2C_Sensor.h"

#include <Adafruit_TSL2561_U.h>


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// TSL2561 Luminosity sensor module
// ###
class Loom_TSL2561 : public LoomI2CSensor
{

protected:

	Adafruit_TSL2561_Unified* inst_TSL2561;		/// Underlying TSL2561 sensor manager instance

	uint8_t 	gain;			/// Gain level
	uint8_t 	resolution;		/// Sensor resolution setting

	uint16_t	lightIR;		/// Measured infra-red lux
	uint16_t	lightFull;		/// Measure full spectrum lux

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// TSL2561 module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x39> | {0x29, 0x39, 0x49} | I2C address
	/// \param[in]	mux_port				Int | <-1> | [0-16] | Port on multiplexer
	/// \param[in]	module_name				String | <"TSL2561"> | null | TSL2561 module name
	/// \param[in]	gain					Set(Int) | <1> | {1, 16} | Gain level
	/// \param[in]	resolution				Set(Int) | <3> | { 1("Low"), 2("Med"), 3("High") } | Resolution
	Loom_TSL2561(
			byte			i2c_address		= 0x39,
			uint8_t			mux_port		= -1,
			const char*		module_name		= "TSL2561",
			uint8_t			gain			= 1,
			uint8_t			resolution		= 3
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_TSL2561(JsonArrayConst p);

	/// Destructor
	~Loom_TSL2561();

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


