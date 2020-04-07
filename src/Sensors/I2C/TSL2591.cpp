///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_TSL2591.cpp
/// @brief		File for Loom_TSL2591 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "TSL2591.h"

#include <Adafruit_Sensor.h>


///////////////////////////////////////////////////////////////////////////////
Loom_TSL2591::Loom_TSL2591(
LoomManager* manager,
const byte i2c_address, 
		const uint8_t		mux_port,
		const uint8_t		gain_level, 
		const uint8_t		timing_level
	)
	: LoomI2CSensor(manager, "TSL2591", Type::TSL2591, i2c_address, mux_port )
	, gain_level(gain_level)
	, timing_level(timing_level)
	, inst_tsl2591( Adafruit_TSL2591(i2c_address) )
{
	bool setup = inst_tsl2591.begin();
		
	if (setup) {
		switch(gain_level) {
			case 0 : inst_tsl2591.setGain(TSL2591_GAIN_LOW);    break;  // 1x gain (bright light)
			case 1 : inst_tsl2591.setGain(TSL2591_GAIN_MED);    break;  // 25x gain
			case 2 : inst_tsl2591.setGain(TSL2591_GAIN_HIGH);   break;  // 428x gain
			case 3 : inst_tsl2591.setGain(TSL2591_GAIN_MAX);    break;  // 9876x gain
			default: LPrintln("Invalid gain level."); break;
		}
	  
		switch(timing_level) {
			case 0 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_100MS); break; // shortest integration time (bright light)
			case 1 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_200MS); break;
			case 2 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_300MS); break;
			case 3 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_400MS); break;
			case 4 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_500MS); break;
			case 5 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light) break;
			default: LPrintln("Invalid timing level"); break;
		}	
	} 

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_TSL2591::Loom_TSL2591(LoomManager* manager, JsonArrayConst p)
	: Loom_TSL2591(manager, EXPAND_ARRAY(p, 4) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_TSL2591::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tVis  : ", vis);
	LPrintln("\tIR   : ", ir);
	LPrintln("\tFull : ", full);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TSL2591::measure()
{
	vis  = inst_tsl2591.getLuminosity(TSL2591_VISIBLE);
	ir   = inst_tsl2591.getLuminosity(TSL2591_INFRARED);
	full = inst_tsl2591.getLuminosity(TSL2591_FULLSPECTRUM);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TSL2591::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["Vis"]  = vis;
	data["IR"]   = ir;
	data["Full"] = full;
}

///////////////////////////////////////////////////////////////////////////////


