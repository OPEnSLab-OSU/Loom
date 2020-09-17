///////////////////////////////////////////////////////////////////////////////
///
/// @file		SHT31D.cpp
/// @brief		File for SHT31D implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "SHT31D.h"
#include "Module_Factory.h"

///////////////////////////////////////////////////////////////////////////////

REGISTER(LoomModule, SHT31D, "SHT31D");

///////////////////////////////////////////////////////////////////////////////
SHT31D::SHT31D(
		const byte		i2c_address,
		const uint8_t	mux_port
	)
	: I2CSensor("SHT31D", Type::SHT31D, i2c_address, mux_port)
{
	bool setup = inst_sht31d.begin(i2c_address);

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
SHT31D::SHT31D(JsonArrayConst p)
	: SHT31D( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void SHT31D::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tTemp  : ", temp, " C");
	LPrintln("\tHumid : ", humid);
}

///////////////////////////////////////////////////////////////////////////////
void SHT31D::measure()
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
void SHT31D::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["temp"]  = temp;
	data["humid"] = humid;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
