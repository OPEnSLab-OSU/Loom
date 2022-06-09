///////////////////////////////////////////////////////////////////////////////
///
/// @file		ADS1232.cpp
/// @brief		File for ADS1232 implementation.
/// @author		Kurtis Dinelle
/// @date		2022
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#define LOOM_INCLUDE_SENSORS // DELETE ME
#ifdef LOOM_INCLUDE_SENSORS

#include "ADS1232.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
ADS1232::ADS1232(
		uint8_t num_samples,
		uint8_t dout,
		uint8_t sclk,
		uint8_t pdwn
	)
	: SPISensor("ADS1232", num_samples)
    , inst_ads(ADS1232_Lib(pdwn, sclk, dout))
{
    this->power_up();
	this->calibrate(); // perhaps automatic
}

///////////////////////////////////////////////////////////////////////////////
ADS1232::ADS1232(JsonArrayConst p)
	: ADS1232(EXPAND_ARRAY(p, 4)) {}

///////////////////////////////////////////////////////////////////////////////
void ADS1232::print_measurements() const
{
	print_module_label();
	LPrintln("\tWeight: ", weight, "g");
}

///////////////////////////////////////////////////////////////////////////////
void ADS1232::measure()
{
	LMark;

    weight = inst_ads.units_read(num_samples);
}

///////////////////////////////////////////////////////////////////////////////
void ADS1232::package(JsonObject json)
{
    LMark;

	JsonObject data = get_module_data_object(json, "ADS1232");
	data["weight"] = weight;
}

///////////////////////////////////////////////////////////////////////////////
void ADS1232::calibrate()
{
    inst_ads.OFFSET = 8403613; // Set to "Calibration offset" from calibration (8527704)
    inst_ads.SCALE = 2041.46; // Set to "Calibration Scale value" from calibration (427.06) 
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
