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
  LMark;
	bool setup = inst_tmp007.begin();
  LMark;

	if (!setup) active = false;
  LMark;

	print_module_label();
  LMark;
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_TMP007::Loom_TMP007(LoomManager* manager, JsonArrayConst p)
	: Loom_TMP007(manager, EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_TMP007::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Measurements:");
  LMark;
	LPrintln("\tObject Temp : ", object_temp);
  LMark;
	LPrintln("\tDie Temp    : ", die_temp);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TMP007::measure()
{
  LMark;
	object_temp	= inst_tmp007.readObjTempC();
  LMark;
	die_temp	= inst_tmp007.readDieTempC();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TMP007::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;

	data["objTemp"] = object_temp;
  LMark;
	data["dieTemp"] = die_temp;
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TMP007::diagnose(bool& result){
  LMark;
	// implement here
}

///////////////////////////////////////////////////////////////////////////////
