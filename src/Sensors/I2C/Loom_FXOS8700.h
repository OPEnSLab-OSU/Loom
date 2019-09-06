#pragma once

#include "Loom_I2C_Sensor.h"

#include <Adafruit_FXOS8700.h>


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// FXOS8700 3-Axis Acceleromter/Magnetometer sensor module
// ###
class Loom_FXOS8700 : public LoomI2CSensor
{

protected:

	Adafruit_FXOS8700	inst_FXOS8700;		/// Underlying FXOS8700 sensor manager instance

//    float        accel[3];                    /// Measured acceleration values (x,y,z) (g's)
//    float        mag[3];                        /// Measured magnetic field values (x,y,z) (T)

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// FXOS8700 module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x1F> | {0x1C, 0x1D, 0x1E, 0x1F} | I2C address
	/// \param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	Loom_FXOS8700(
			byte			i2c_address		= 0x1F,
			uint8_t			mux_port		= 255
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_FXOS8700(JsonArrayConst p);

	/// Destructor
	~Loom_FXOS8700() = default;

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


