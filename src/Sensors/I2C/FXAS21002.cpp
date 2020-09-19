///////////////////////////////////////////////////////////////////////////////
///
/// @file		FXAS21002.cpp
/// @brief		File for FXAS21002 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "FXAS21002.h"
#include "Module_Factory.h"

#include <Adafruit_Sensor.h>

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
FXAS21002::FXAS21002(
		const byte		i2c_address, 
		const uint8_t	mux_port
	)
	: I2CSensor("FXAS21002", i2c_address, mux_port)
	, inst_FXAS21002(Adafruit_FXAS21002C(0x0021002C))
{
	bool setup = inst_FXAS21002.begin();

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
FXAS21002::FXAS21002(JsonArrayConst p)
	: FXAS21002(EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void FXAS21002::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tgx: ", gyro[0]);
	LPrintln("\tgy: ", gyro[1]);
	LPrintln("\tgz: ", gyro[2]);
}

///////////////////////////////////////////////////////////////////////////////
void FXAS21002::measure()
{
	sensors_event_t event;
	inst_FXAS21002.getEvent(&event);

	gyro[0] = event.gyro.x;
	gyro[1] = event.gyro.y;
	gyro[2] = event.gyro.z;
}

///////////////////////////////////////////////////////////////////////////////
void FXAS21002::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	data["gx"] = gyro[0];
	data["gy"] = gyro[1];
	data["gz"] = gyro[2];
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS

