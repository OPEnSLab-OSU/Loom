///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_TMP007.cpp
/// @brief		File for Loom_TMP007 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "TMP007.h"


///////////////////////////////////////////////////////////////////////////////
Loom_TMP007::Loom_TMP007(
LoomManager* manager,
const byte i2c_address, 
		const uint8_t		mux_port
	) 
	: LoomI2CSensor(manager, "TMP007", Type::TMP007, i2c_address, mux_port )
	, inst_tmp007( Adafruit_TMP007(i2c_address) )
{
	bool setup = inst_tmp007.begin();

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_TMP007::Loom_TMP007(LoomManager* manager, JsonArrayConst p)
	: Loom_TMP007(manager, EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_TMP007::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tObject Temp : ", object_temp);
	LPrintln("\tDie Temp    : ", die_temp);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TMP007::measure()
{
	object_temp	= inst_tmp007.readObjTempC();
	die_temp	= inst_tmp007.readDieTempC();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TMP007::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["objTemp"] = object_temp;
	data["dieTemp"] = die_temp;
}

///////////////////////////////////////////////////////////////////////////////
	