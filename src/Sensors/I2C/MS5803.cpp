///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_MS5803.cpp
/// @brief		File for Loom_MS5803 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "MS5803.h"


// #include <setjmp.h>     /* jmp_buf, setjmp, longjmp */
// #include <RTCCounter.h>

// jmp_buf env;
// void escape() {
// 	Serial.println("escape");
// 	delay(50);
// 	longjmp(env,101);
// }


///////////////////////////////////////////////////////////////////////////////
Loom_MS5803::Loom_MS5803(
LoomManager* manager,
const byte i2c_address, 
		const uint8_t			mux_port
	)
	: LoomI2CSensor(manager, "MS5803", Type::MS5803, i2c_address, mux_port )
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
Loom_MS5803::Loom_MS5803(LoomManager* manager, JsonArrayConst p)
	: Loom_MS5803(manager, EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MS5803::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tPressure    : ", pressure , " mbar");
	LPrintln("\tTemperature : ", temp     , " C");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MS5803::measure()
{
	inst_MS5803.readSensor();

	pressure = inst_MS5803.pressure();
	temp     = inst_MS5803.temperature();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MS5803::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["pressure"] = pressure;
	data["temp"]     = temp;
}

///////////////////////////////////////////////////////////////////////////////





