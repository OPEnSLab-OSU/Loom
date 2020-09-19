///////////////////////////////////////////////////////////////////////////////
///
/// @file		TMP007.cpp
/// @brief		File for TMP007 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "TMP007.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
TMP007::TMP007(
		const byte		i2c_address, 
		const uint8_t	mux_port
	) 
	: I2CSensor("TMP007", i2c_address, mux_port)
	, inst_tmp007( Adafruit_TMP007(i2c_address) )
{
	bool setup = inst_tmp007.begin();

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
TMP007::TMP007(JsonArrayConst p)
	: TMP007(EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void TMP007::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tObject Temp : ", object_temp);
	LPrintln("\tDie Temp    : ", die_temp);
}

///////////////////////////////////////////////////////////////////////////////
void TMP007::measure()
{
	object_temp	= inst_tmp007.readObjTempC();
	die_temp	= inst_tmp007.readDieTempC();
}

///////////////////////////////////////////////////////////////////////////////
void TMP007::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["objTemp"] = object_temp;
	data["dieTemp"] = die_temp;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
