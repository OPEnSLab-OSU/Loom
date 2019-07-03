
#include "Loom_SHT31D.h"


///////////////////////////////////////////////////////////////////////////////
Loom_SHT31D::Loom_SHT31D(
		byte			i2c_address,
		const char*		module_name
	)
	: LoomI2CSensor( module_name, i2c_address )
{
	this->module_type = ModuleType::SHT31D;

	bool setup = inst_sht31d.begin(i2c_address);

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_SHT31D::Loom_SHT31D(JsonVariant p)
	: Loom_SHT31D( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_SHT31D::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Temperature: ", temp, " C");
	LPrintln("\t", "Humidity   : ", humid);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SHT31D::measure()
{
	float t = inst_sht31d.readTemperature();
	float h = inst_sht31d.readHumidity();

	if ((!isnan(t)) && (!isnan(h))) {
		temp = t;
		humid = h;
	} else {
		print_module_label();
		LPrintln("Failed to read temperature or humidity");
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SHT31D::package(JsonObject json)
{
	package_json(json, module_name, 
		"Temp",		temp,
		"Humid",	humid
	);
}

///////////////////////////////////////////////////////////////////////////////

