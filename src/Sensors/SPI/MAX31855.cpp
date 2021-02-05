///////////////////////////////////////////////////////////////////////////////
///
/// @file		MAX31855.cpp
/// @brief		File for MAX31855 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "MAX31855.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
MAX31855::MAX31855(
		const char*		module_name,
		const uint8_t	num_samples,
		const uint8_t	CS_pin
	)
	: SPISensor(module_name, num_samples)
	, inst_max(CS_pin)
{
  LMark;
	inst_max.begin();
}

///////////////////////////////////////////////////////////////////////////////
MAX31855::MAX31855(JsonArrayConst p)
	: MAX31855(EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void MAX31855::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("\tTemperature   : ", temperature, " °C");
  LMark;
	LPrintln("\tInternal Temp : ", internal_temp, " °C");
}

///////////////////////////////////////////////////////////////////////////////
void MAX31855::measure()
{
  LMark;
	int i = num_samples;
  LMark;
	float int_temp = 0, temp = 0, t;
  LMark;

	while (i--) {
   	LMark;
		int_temp += inst_max.readInternal();
   	LMark;
		t = inst_max.readCelsius();
   	LMark;
		if (isnan(t)) {
    	LMark;
			print_module_label();
    	LMark;
			LPrintln("Something wrong with thermocouple!");
    	LMark;
			break;
		} else {
			temp += t;
		}
	}

	internal_temp = int_temp / num_samples;
  LMark;
	temperature   = temp / num_samples;
}

///////////////////////////////////////////////////////////////////////////////
void MAX31855::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;
	data["temp"] = temperature;
  LMark;

	if (package_verbosity == Verbosity::V_HIGH) {
   	LMark;
		data["internal"] = internal_temp;
	}
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
