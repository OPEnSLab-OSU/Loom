
#include "Loom_SHT31D.h"


///////////////////////////////////////////////////////////////////////////////
Loom_SHT31D::Loom_SHT31D(
		byte			i2c_address,
		uint8_t			mux_port,
		const char*		module_name
	)
	: LoomI2CSensor( module_name, Type::SHT31D, i2c_address, mux_port )
{
	bool setup = inst_sht31d.begin(i2c_address);

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_SHT31D::Loom_SHT31D(JsonArrayConst p)
	: Loom_SHT31D( EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_SHT31D::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tTemp  : ", temp, " C");
	LPrintln("\tHumid : ", humid);
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
		LPrintln("Failed to read temp/humid");
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SHT31D::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["temp"]  = temp;
	data["humid"] = humid;
}

///////////////////////////////////////////////////////////////////////////////

