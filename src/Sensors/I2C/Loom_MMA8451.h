#pragma once

#include "Loom_I2C_Sensor.h"

#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// MMA8451 accelerometer
// ###
class Loom_MMA8451 : public LoomI2CSensor
{

protected:

	Adafruit_MMA8451 MMA;			/// Underlying MMA8451 sensor manager instance

	float			accel[3];		///	Acceleration values for each axis
	uint8_t			orientation;	/// Orientation

	mma8451_range_t	range; 			/// Range setting (2/4/8 G)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x1D> | {0x1C, 0x1D} | I2C address
	/// \param[in]	module_name				String | <"MMA8451"> | null | MMA8451 module name
	/// \param[in]	range					Set() | <"MMA8451"> | null | MMA8451 module name
	/// \param[in]	type					Set(Version) | <0> | {0("Featherwing"), 1("Breakout")} | Which version of the OLED is being used

	Loom_MMA8451(
			byte			i2c_address		= 0x1D,
			const char*		module_name		= "MMA8451",
			mma8451_range_t	range			= MMA8451_RANGE_2_G
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_MMA8451(JsonArrayConst p);
	
	/// Destructor
	virtual ~Loom_MMA8451() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_measurements() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================


//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================


//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================



private:

};




