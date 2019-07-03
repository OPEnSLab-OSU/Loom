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

	int 		gain;			/// Gain level
	int 		resolution;		/// Sensor resolution setting

	int 		lightIR;		/// Measured infra-red lux
	int 		lightFull;		/// Measure full spectrum lux

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// TSL2561 module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x29> | {0x29, 0x39, 0x49} | I2C address
	/// \param[in]	module_name				String | <"TSL2561"> | null | TSL2561 module name
	/// \param[in]	gain					Set(Int) | <1> | {1, 16} | Gain level
	/// \param[in]	resolution				Set(Int) | <3> | { 1("Low"), 2("Med"), 3("High") } | Resolution
	Loom_TSL2561(
			byte			i2c_address		= 0x39,
			const char*		module_name		= "TSL2561",
			int				gain			= 1,
			int				resolution		= 3
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_TSL2561(JsonVariant p);

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


