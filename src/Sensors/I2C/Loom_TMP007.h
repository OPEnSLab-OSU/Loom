#pragma once

#include "Loom_I2C_Sensor.h"

#include <Adafruit_TMP007.h>


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// TMP007 thermopile temperature sensor module
// ###
class Loom_TMP007 : public LoomI2CSensor
{

protected:

	Adafruit_TMP007 inst_tmp007;	///< Underlying TMP007 sensor manager instance

	float		object_temp;		///< Object temperature
	float		die_temp;			///< Die temperature

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x40> | {0x40, 0x41, 0x42, 0x43, 0x46, 0x47, 0x48} | I2C address
	/// \param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	Loom_TMP007(
			byte			i2c_address		= 0x40,
			uint8_t			mux_port		= 255
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_TMP007(JsonArrayConst p);
	
	/// Destructor
	virtual ~Loom_TMP007() = default;

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




