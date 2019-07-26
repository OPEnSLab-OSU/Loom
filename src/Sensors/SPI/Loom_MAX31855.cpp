
#include "Loom_MAX31855.h"


///////////////////////////////////////////////////////////////////////////////
Loom_MAX31855::Loom_MAX31855(	
		const char*		module_name,
		uint8_t			num_samples, 
		uint8_t			CS_pin
	) 
	: LoomSPISensor( module_name, Type::MAX31855, num_samples ) 
	, inst_max(CS_pin)
{
	inst_max.begin();
}

///////////////////////////////////////////////////////////////////////////////
Loom_MAX31855::Loom_MAX31855(JsonArrayConst p)
	: Loom_MAX31855( EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31855::print_measurements() 
{
	print_module_label();
	LPrintln();
	LPrintln("\tTemperature   : ", temperature, " °C");
	LPrintln("\tInternal Temp : ", internal_temp, " °C");
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

	internal_temp = int_temp / num_samples;
	temperature   = temp / num_samples;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31855::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	data["temp"] = temperature;

	if (package_verbosity == Verbosity::V_HIGH) {
		data["internal"] = internal_temp;
	}
}

///////////////////////////////////////////////////////////////////////////////

