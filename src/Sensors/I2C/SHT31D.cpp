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

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
SHT31D::SHT31D(
		const byte		i2c_address,
		const uint8_t	mux_port
	)
	: I2CSensor("SHT31D", i2c_address, mux_port)
{
  LMark;
	bool setup = inst_sht31d.begin(i2c_address);

	if (!setup) active = false;
  LMark;

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
  LMark;
	LPrintln("\tTemp  : ", temp, " C");
  LMark;
	LPrintln("\tHumid : ", humid);
}

///////////////////////////////////////////////////////////////////////////////
void SHT31D::measure()
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
	} else {
		print_module_label();
		LPrintln("Failed to read temp/humid");
	}
}

///////////////////////////////////////////////////////////////////////////////
void SHT31D::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;

	data["temp"]  = temp;
  LMark;
	data["humid"] = humid;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
