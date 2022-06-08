///////////////////////////////////////////////////////////////////////////////
///
/// @file		MAX31865.h
/// @brief		File for MAX31865 definition.
/// @author		Kurtis Dinelle
/// @date		2022
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "SPI_Sensor.h"

#include <Adafruit_MAX31865.h>

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
/// 
/// MAX31865 RTD module. Measures temperature.
/// 
/// @par Resources
/// - [Module Documentation](<Does not exist yet>)
/// - [Product Page: Adafruit PT1000 RTD Temperature Sensor Amplifier](https://www.adafruit.com/product/3648)
/// - [Dependency: Adafruit_MAX31865](https://github.com/adafruit/Adafruit_MAX31865)
/// 
//////////////////////////////////////////////////////////////////////////////
class MAX31865 : public SPISensor
{

protected:
    Adafruit_MAX31865 inst_max;
    float temperature;

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// MAX31865 module constructor.
	///
	/// @param[in]	num_samples			Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
	/// @param[in]	CS_PIN				Set(Int) | <10> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin should be used for chip select
	MAX31865(
		uint8_t	num_samples		        = 1,
        uint8_t CS_pin                  = 10
	);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	MAX31865(JsonArrayConst p);

	/// Destructor
	virtual ~MAX31865() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_measurements() const override;

private:

};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, MAX31865, "MAX31865");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS
