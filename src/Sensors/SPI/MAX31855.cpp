///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_MAX31855.cpp
/// @brief		File for Loom_MAX31855 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "MAX31855.h"


///////////////////////////////////////////////////////////////////////////////
Loom_MAX31855::Loom_MAX31855(
		LoomManager* manager,
		const char* module_name,
		const uint8_t		num_samples,
		const uint8_t		CS_pin
	)
	: LoomSPISensor(manager, module_name, Type::MAX31855, num_samples )
	, inst_max(CS_pin)
{
  LMark;
	inst_max.begin();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_MAX31855::Loom_MAX31855(LoomManager* manager, JsonArrayConst p)
	: Loom_MAX31855(manager, EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31855::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("\tTemperature   : ", temperature, " °C");
  LMark;
	LPrintln("\tInternal Temp : ", internal_temp, " °C");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31855::measure()
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
   		LMark;
		}
	}

	internal_temp = int_temp / num_samples;
  LMark;
	temperature   = temp / num_samples;
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31855::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;
	data["temp"] = temperature;
  LMark;

	if (package_verbosity == Verbosity::V_HIGH) {
   	LMark;
		data["internal"] = internal_temp;
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31855::diagnose(bool& result){
  LMark;
	// implement here
}

///////////////////////////////////////////////////////////////////////////////
