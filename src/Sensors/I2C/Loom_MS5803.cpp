
#include "Loom_MS5803.h"


///////////////////////////////////////////////////////////////////////////////
Loom_MS5803::Loom_MS5803(
		byte			i2c_address, 
		uint8_t			mux_port
	)
	: LoomI2CSensor( "MS5803", Type::MS5803, i2c_address, mux_port )
	, inst_MS5803( MS_5803(i2c_address, 512) )
{

	// inst_MS5803 = new MS_5803(i2c_address, 512);
	bool setup = !inst_MS5803.initializeMS_5803(); // for some reason this library returns setup incorrectly

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_MS5803::Loom_MS5803(JsonArrayConst p)
	: Loom_MS5803( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MS5803::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tPressure    : ", pressure , " mbar");
	LPrintln("\tTemperature : ", temp     , " C");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MS5803::measure()
{
	inst_MS5803.readSensor();

	pressure = inst_MS5803.pressure();
	temp     = inst_MS5803.temperature();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MS5803::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["pressure"] = pressure;
	data["temp"]     = temp;
}

///////////////////////////////////////////////////////////////////////////////

