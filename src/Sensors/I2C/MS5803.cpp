///////////////////////////////////////////////////////////////////////////////
///
/// @file		MS5803.cpp
/// @brief		File for MS5803 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "MS5803.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
MS5803::MS5803(
		const byte		i2c_address, 
		const uint8_t	mux_port
	)
	: I2CSensor("MS5803", i2c_address, mux_port)
	, inst_MS5803( MS_5803(i2c_address, 512) )
{

	// rtcCounter.begin();
	// rtcCounter.attachInterrupt(escape);
	// rtcCounter.setPeriodicAlarm(10);


	// jmp_buf env;
	// int val;
	// bool setup;
	// // int val = setjmp (env);
	// if ( !setjmp(env) ) {
	// 	inst_MS5803.initializeMS_5803(); // for some reason this library returns setup incorrectly
	// 	setup = true;
	// 	// setup = !inst_MS5803.initializeMS_5803(); // for some reason this library returns setup incorrectly
	// 	print_module_label();
	// 	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
	// } else {
	// 	LPrintln("Error happened");
	// 	setup = false;
	// }


	// rtcCounter.detachInterrupt();
	// // inst_MS5803 = new MS_5803(i2c_address, 512);

	// if (!setup) active = false;

	/////////

	bool setup = !inst_MS5803.initializeMS_5803(); // for some reason this library returns setup incorrectly

	// if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");

}

///////////////////////////////////////////////////////////////////////////////
MS5803::MS5803(JsonArrayConst p)
	: MS5803( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void MS5803::add_config(JsonObject json)
{
	JsonArray params = add_config_temp(json, module_name);
	params.add(i2c_address);
	params.add(port_num);
}

///////////////////////////////////////////////////////////////////////////////
void MS5803::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tPressure    : ", pressure , " mbar");
	LPrintln("\tTemperature : ", temp     , " C");
}

///////////////////////////////////////////////////////////////////////////////
void MS5803::measure()
{
	inst_MS5803.readSensor();

	pressure = inst_MS5803.pressure();
	temp     = inst_MS5803.temperature();
}

///////////////////////////////////////////////////////////////////////////////
void MS5803::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["pressure"] = pressure;
	data["temp"]     = temp;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS



