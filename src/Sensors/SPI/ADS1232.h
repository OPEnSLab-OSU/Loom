///////////////////////////////////////////////////////////////////////////////
///
/// @file		ADS1232.h
/// @brief		File for ADS1232 definition.
/// @author		Kurtis Dinelle
/// @date		2022
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "SPI_Sensor.h"

#include <ADS1232_Lib.h>

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
/// 
/// ADS1232 Sensor
///
/// @par Resources
/// - [Module Documentation](<Does not exist yet>)
/// - [Product Page: <Name>](<Link>)
/// - [Dependency: ADS1232](https://github.com/ciorceri/ADS1232)
///
///////////////////////////////////////////////////////////////////////////////
class ADS1232 : public SPISensor
{

protected:
	ADS1232_Lib inst_ads;
	float weight;
	long offset;
	float scale;

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// ADS1232 Constructor
	///
	/// @param[in] num_samples | How many samples to take and average
	/// @param[in] offset | Calibration offset value
	/// @param[in] scale | Calibration scale value
	ADS1232(
		uint8_t num_samples	= 1,
		long offset			= 8403613,
		float scale			= 2041.46
	);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	ADS1232(JsonArrayConst p);

	/// Destructor
	virtual ~ADS1232() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;
	void		calibrate() override;
	void		power_down() override { inst_ads.power_down(); }
	void		power_up() override { inst_ads.power_up(); }

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_measurements() const override;

private:

};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, ADS1232, "ADS1232");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS
