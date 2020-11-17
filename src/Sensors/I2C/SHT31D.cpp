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
  LMark;
	bool setup = inst_sht31d.begin(i2c_address);
  LMark;

	if (!setup) active = false;
  LMark;

	print_module_label();
  LMark;
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_SHT31D::Loom_SHT31D(LoomManager* manager, JsonArrayConst p)
	: Loom_SHT31D(manager, EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_SHT31D::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Measurements:");
  LMark;
	LPrintln("\tTemp  : ", temp, " C");
  LMark;
	LPrintln("\tHumid : ", humid);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SHT31D::measure()
{
  LMark;
	float t = inst_sht31d.readTemperature();
  LMark;
	float h = inst_sht31d.readHumidity();
  LMark;

	if ((!isnan(t)) && (!isnan(h))) {
   	LMark;
		temp = t;
   	LMark;
		humid = h;
  	LMark;
	} else {
		print_module_label();
   	LMark;
		LPrintln("Failed to read temp/humid");
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SHT31D::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;

	data["temp"]  = temp;
  LMark;
	data["humid"] = humid;
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SHT31D::diagnose(bool& result){
  LMark;
	// implement here
}

///////////////////////////////////////////////////////////////////////////////
