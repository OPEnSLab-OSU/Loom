///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_FXAS21002.cpp
/// @brief		File for Loom_FXAS21002 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "FXAS21002.h"

#include <Adafruit_Sensor.h>


///////////////////////////////////////////////////////////////////////////////
Loom_FXAS21002::Loom_FXAS21002(
LoomManager* manager,
const byte i2c_address,
		const uint8_t		mux_port
	)
	: LoomI2CSensor(manager, "FXAS21002", Type::FXAS21002, i2c_address, mux_port )
	, inst_FXAS21002(Adafruit_FXAS21002C(0x0021002C))
{
  LMark;
	bool setup = inst_FXAS21002.begin();
  LMark;

	if (!setup) active = false;
  LMark;

	print_module_label();
  LMark;
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_FXAS21002::Loom_FXAS21002(LoomManager* manager, JsonArrayConst p)
	: Loom_FXAS21002(manager, EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXAS21002::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Measurements:");
  LMark;
	LPrintln("\tgx: ", gyro[0]);
  LMark;
	LPrintln("\tgy: ", gyro[1]);
  LMark;
	LPrintln("\tgz: ", gyro[2]);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXAS21002::measure()
{
  LMark;
	sensors_event_t event;
  LMark;
	inst_FXAS21002.getEvent(&event);
  LMark;

	gyro[0] = event.gyro.x;
  LMark;
	gyro[1] = event.gyro.y;
  LMark;
	gyro[2] = event.gyro.z;
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXAS21002::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;
	data["gx"] = gyro[0];
  LMark;
	data["gy"] = gyro[1];
  LMark;
	data["gz"] = gyro[2];
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXAS21002::diagnose(bool& result){
  LMark;
	// implement here
}

///////////////////////////////////////////////////////////////////////////////
