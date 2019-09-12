///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_FXAS21002.cpp
/// @brief		File for Loom_FXAS21002 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Loom_FXAS21002.h"

#include <Adafruit_Sensor.h>

///////////////////////////////////////////////////////////////////////////////
Loom_FXAS21002::Loom_FXAS21002(
		byte			i2c_address, 
		uint8_t			mux_port
	)
	: LoomI2CSensor( "FXAS21002", Type::FXAS21002, i2c_address, mux_port )
	, inst_FXAS21002(Adafruit_FXAS21002C(0x0021002C))
{
	bool setup = inst_FXAS21002.begin();
    
    for(int i = 0; i < 3; i++) {
        Values.push_back(var());
    }

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_FXAS21002::Loom_FXAS21002(JsonArrayConst p)
	: Loom_FXAS21002( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXAS21002::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tgx: ", Values[0].retrieve<float>().value_or(0));
	LPrintln("\tgy: ", Values[1].retrieve<float>().value_or(0));
	LPrintln("\tgz: ", Values[2].retrieve<float>().value_or(0));
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXAS21002::measure()
{
	sensors_event_t event;
	inst_FXAS21002.getEvent(&event);

	Values[0] = event.gyro.x;
	Values[1] = event.gyro.y;
	Values[2] = event.gyro.z;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXAS21002::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	data["gx"] = Values[0].retrieve<float>().value_or(0);
	data["gy"] = Values[1].retrieve<float>().value_or(0);
	data["gz"] = Values[2].retrieve<float>().value_or(0);
}

///////////////////////////////////////////////////////////////////////////////



