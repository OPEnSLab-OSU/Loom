#pragma once

#include "Loom_I2C_Sensor.h"

#include <Adafruit_FXAS21002C.h>


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// FXAS21002 3-Axis Gyroscope snesor module
// ###
class Loom_FXAS21002 : public LoomI2CSensor
{

protected:
	
	Adafruit_FXAS21002C		inst_FXAS21002;		///< Underlying FXAS21002 sensor manager instance

	float					gyro[3];			///< Measured gyro values (x,y,z)

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// FXAS21002 module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x20> | {0x20, 0x21} | I2C address
	/// \param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	Loom_FXAS21002(
			byte			i2c_address		= 0x20,
			uint8_t			mux_port		= 255
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_FXAS21002(JsonArrayConst p);

	/// Destructor
	~Loom_FXAS21002() = default;

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


