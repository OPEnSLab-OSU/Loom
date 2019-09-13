///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_SHT31D.cpp
/// @brief		File for Loom_SHT31D implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "Loom_SHT31D.h"


///////////////////////////////////////////////////////////////////////////////
Loom_SHT31D::Loom_SHT31D(
		const byte			i2c_address,
		const uint8_t		mux_port
	)
	: LoomI2CSensor( "SHT31D", Type::SHT31D, i2c_address, mux_port )
{
	bool setup = inst_sht31d.begin(i2c_address);
    
    for(int i = 0; i < 2; i++) {
        Values.push_back(var());
    }

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_SHT31D::Loom_SHT31D(JsonArrayConst p)
	: Loom_SHT31D( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_SHT31D::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tTemp  : ", Values[0].retrieve<float>().value_or(0), " C");
	LPrintln("\tHumid : ", Values[1].retrieve<float>().value_or(0));
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SHT31D::measure()
{
	float t = inst_sht31d.readTemperature();
	float h = inst_sht31d.readHumidity();

	if ((!isnan(t)) && (!isnan(h))) {
		Values[0] = t;
		Values[1] = h;
	} else {
		print_module_label();
		LPrintln("Failed to read temp/humid");
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SHT31D::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["temp"]  = Values[0].retrieve<float>().value_or(0);
	data["humid"] = Values[1].retrieve<float>().value_or(0);
}

///////////////////////////////////////////////////////////////////////////////

