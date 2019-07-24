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
	
	Adafruit_SHT31 inst_sht31d;		/// Underlying SHT31D sensor manager instance

	float		temp;				/// Measured temperature (C)
	float		humid;				/// Measured humidity (%)

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// SHT31D module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x44> | {0x44, 0x45} | I2C address
	/// \param[in]	mux_port				Int | <-1> | [0-16] | Port on multiplexer
	/// \param[in]	module_name				String | <"SHT31D"> | null | SHT31D module name
	Loom_SHT31D(
			byte			i2c_address		= 0x44,
			int				mux_port		= -1,
			const char*		module_name		= "SHT31D"
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


