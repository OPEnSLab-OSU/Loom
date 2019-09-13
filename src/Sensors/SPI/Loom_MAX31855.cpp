///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_MAX31855.cpp
/// @brief		File for Loom_MAX31855 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "Loom_MAX31855.h"


///////////////////////////////////////////////////////////////////////////////
Loom_MAX31855::Loom_MAX31855(	
		const char*			module_name,
		const uint8_t		num_samples, 
		const uint8_t		CS_pin
	) 
	: LoomSPISensor( module_name, Type::MAX31855, num_samples ) 
	, inst_max(CS_pin)
{
	inst_max.begin();
    
    for (int i = 0; i < 2; i++) {
        Values.push_back(var());
    }
}

///////////////////////////////////////////////////////////////////////////////
Loom_MAX31855::Loom_MAX31855(JsonArrayConst p)
	: Loom_MAX31855( EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31855::print_measurements() const 
{
	print_module_label();
	LPrintln();
	LPrintln("\tTemperature   : ", Values[0].retrieve<float>().value_or(0), " °C");
	LPrintln("\tInternal Temp : ", Values[1].retrieve<float>().value_or(0), " °C");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31855::measure() 
{
	int i = num_samples;
	float int_temp = 0, temp = 0, t;

	while (i--) {
		int_temp += inst_max.readInternal();
		t = inst_max.readCelsius();
		if (isnan(t)) {
			print_module_label();
			LPrintln("Something wrong with thermocouple!");
			break;
		} else {
			temp += t;
		}
	}

	Values[1] = int_temp / num_samples;
	Values[0]   = temp / num_samples;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31855::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	data["temp"] = Values[0].retrieve<float>().value_or(0);

	if (package_verbosity == Verbosity::V_HIGH) {
		data["internal"] = Values[1].retrieve<float>().value_or(0);
	}
}

///////////////////////////////////////////////////////////////////////////////

