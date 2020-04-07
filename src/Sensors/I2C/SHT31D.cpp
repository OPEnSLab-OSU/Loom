///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_SHT31D.cpp
/// @brief		File for Loom_SHT31D implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "SHT31D.h"


///////////////////////////////////////////////////////////////////////////////
Loom_SHT31D::Loom_SHT31D(
LoomManager* manager,
const byte i2c_address,
		const uint8_t		mux_port
	)
	: LoomI2CSensor(manager, "SHT31D", Type::SHT31D, i2c_address, mux_port )
{
	bool setup = inst_sht31d.begin(i2c_address);

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_SHT31D::Loom_SHT31D(LoomManager* manager, JsonArrayConst p)
	: Loom_SHT31D(manager, EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_SHT31D::print_measurements() const
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

