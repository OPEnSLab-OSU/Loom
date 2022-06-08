///////////////////////////////////////////////////////////////////////////////
///
/// @file		MAX31865.cpp
/// @brief		File for MAX31865 implementation.
/// @author		Kurtis Dinelle
/// @date		2022
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "MAX31865.h"
#include "Module_Factory.h"

#define RNOMINAL    1000.0
#define RREF        4300.0

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
MAX31865::MAX31865(
		uint8_t				    num_samples,
        uint8_t                 CS_pin
	)
	: SPISensor("MAX31865", num_samples)
    , inst_max(Adafruit_MAX31865(CS_pin))
{
    LMark;
	
    inst_max.begin(MAX31865_2WIRE);
}

///////////////////////////////////////////////////////////////////////////////
MAX31865::MAX31865(JsonArrayConst p)
	: MAX31865(EXPAND_ARRAY(p, 2)) {}

///////////////////////////////////////////////////////////////////////////////
void MAX31865::print_measurements() const
{
	print_module_label();
	LPrintln("\tTemp: ", temperature, " C");
}

///////////////////////////////////////////////////////////////////////////////
void MAX31865::measure()
{
    LMark;

	int i = num_samples;
	float temp = 0;

	while (i--) {
		temp += inst_max.temperature(RNOMINAL, RREF);

        // Check and print any faults
        uint8_t fault = inst_max.readFault();
        if (fault && (print_verbosity == Verbosity::V_HIGH)) {
            if (fault & MAX31865_FAULT_HIGHTHRESH)  LPrintln("RTD High Threshold"); 
            if (fault & MAX31865_FAULT_LOWTHRESH)   LPrintln("RTD Low Threshold"); 
            if (fault & MAX31865_FAULT_REFINLOW)    LPrintln("REFIN- > 0.85 x Bias"); 
            if (fault & MAX31865_FAULT_REFINHIGH)   LPrintln("REFIN- < 0.85 x Bias - FORCE- open"); 
            if (fault & MAX31865_FAULT_RTDINLOW)    LPrintln("RTDIN- < 0.85 x Bias - FORCE- open"); 
            if (fault & MAX31865_FAULT_OVUV)        LPrintln("Under/Over voltage"); 
            break;
        }
	}

	temperature = temp / num_samples;
}

///////////////////////////////////////////////////////////////////////////////
void MAX31865::package(JsonObject json)
{
    LMark;

	JsonObject data = get_module_data_object(json, "MAX31865");
	data["temp"] = temperature;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
