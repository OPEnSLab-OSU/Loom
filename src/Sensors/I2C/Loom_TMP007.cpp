///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_TMP007.cpp
/// @brief		File for Loom_TMP007 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "Loom_TMP007.h"


///////////////////////////////////////////////////////////////////////////////
Loom_TMP007::Loom_TMP007(
		const byte			i2c_address, 
		const uint8_t		mux_port
	) 
	: LoomI2CSensor( "TMP007", Type::TMP007, i2c_address, mux_port )
	, inst_tmp007( Adafruit_TMP007(i2c_address) )
{
	bool setup = inst_tmp007.begin();
    
    for(int i = 0; i < 2; i++) {
        Values.push_back(var());
    }

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_TMP007::Loom_TMP007(JsonArrayConst p)
	: Loom_TMP007( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_TMP007::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tObject Temp : ", Values[0].retrieve<float>().value_or(0));
	LPrintln("\tDie Temp    : ", Values[1].retrieve<float>().value_or(0));
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TMP007::measure()
{
	Values[0] = inst_tmp007.readObjTempC(); //< object_temp
	Values[1] = inst_tmp007.readDieTempC(); //< die_temp
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TMP007::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["objTemp"] = Values[0].retrieve<float>().value_or(0);
	data["dieTemp"] = Values[1].retrieve<float>().value_or(0);
}

///////////////////////////////////////////////////////////////////////////////
