#pragma once

#include "Loom_I2C_Sensor.h"

#include <SparkFunLIS3DH.h>


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// LIS3DH Accelerometer sensor module.
/// Needs more work on properly implementing support for the sensors interrupts 
// ###
class Loom_LIS3DH : public LoomI2CSensor
{

protected:

	LIS3DH		inst_LIS3DH;	///< Underlying LIS3DH sensor manager instance

	float		accel[3];		///< Measured acceleration values (x,y,z)(g's)

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// LIS3DH module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x19> | {0x19} | I2C address
	/// \param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	Loom_LIS3DH(
			byte			i2c_address		= 0x19,
			uint8_t			mux_port		= 255
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_LIS3DH(JsonArrayConst p);

	/// Destructor
	~Loom_LIS3DH() = default;

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



