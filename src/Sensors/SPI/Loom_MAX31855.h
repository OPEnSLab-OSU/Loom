#pragma once

#include "Loom_SPI_Sensor.h"

#include <Adafruit_MAX31855.h>


///////////////////////////////////////////////////////////////////////////////


// ### (LoomSPISensor) | dependencies: [] | conflicts: []
/// MAX31855 thermocouple temperature module.
// ###
class Loom_MAX31855 : public LoomSPISensor
{

protected:

	Adafruit_MAX31855 inst_max;		/// Underlying MAX31855 sensor manager instance

//    float temperature;                /// Measured temperature
//    float internal_temp;            /// Measured internal temperature

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// MAX31856 module constructor.
	///
	/// \param[in]	module_name			String | <"MAX31856"> | null | MAX31856 module name
	/// \param[in]	num_samples			Set(Int) | <4> | {1, 2, 4, 8, 16} | How many samples to take and average
	/// \param[in]	CS_pin				Set(Int) | <10> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin should be used for chip select
	Loom_MAX31855(
			const char*		module_name			= "MAX31855",
			uint8_t			num_samples			= 4,
			uint8_t			CS_pin				= 10
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_MAX31855(JsonArrayConst p);

	/// Destructor
	~Loom_MAX31855() = default;

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

