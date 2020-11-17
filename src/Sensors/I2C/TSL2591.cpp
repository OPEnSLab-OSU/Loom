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
  LMark;
	bool setup = inst_tsl2591.begin();
  LMark;

	if (setup) {
   	LMark;
		switch(gain_level) {
    	LMark;
			case 0 : inst_tsl2591.setGain(TSL2591_GAIN_LOW);    break;  // 1x gain (bright light)
    	LMark;
			case 1 : inst_tsl2591.setGain(TSL2591_GAIN_MED);    break;  // 25x gain
    	LMark;
			case 2 : inst_tsl2591.setGain(TSL2591_GAIN_HIGH);   break;  // 428x gain
    	LMark;
			case 3 : inst_tsl2591.setGain(TSL2591_GAIN_MAX);    break;  // 9876x gain
    	LMark;
			default: LPrintln("Invalid gain level."); break;
   	LMark;
		}

		switch(timing_level) {
    	LMark;
			case 0 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_100MS); break; // shortest integration time (bright light)
    	LMark;
			case 1 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_200MS); break;
    	LMark;
			case 2 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_300MS); break;
    	LMark;
			case 3 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_400MS); break;
    	LMark;
			case 4 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_500MS); break;
    	LMark;
			case 5 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light) break;
    	LMark;
			default: LPrintln("Invalid timing level"); break;
   		LMark;
		}
	}

	if (!setup) active = false;
  LMark;

	print_module_label();
  LMark;
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_TSL2591::Loom_TSL2591(LoomManager* manager, JsonArrayConst p)
	: Loom_TSL2591(manager, EXPAND_ARRAY(p, 4) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_TSL2591::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Measurements:");
  LMark;
	LPrintln("\tVis  : ", vis);
  LMark;
	LPrintln("\tIR   : ", ir);
  LMark;
	LPrintln("\tFull : ", full);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TSL2591::measure()
{
  LMark;
	vis  = inst_tsl2591.getLuminosity(TSL2591_VISIBLE);
  LMark;
	ir   = inst_tsl2591.getLuminosity(TSL2591_INFRARED);
  LMark;
	full = inst_tsl2591.getLuminosity(TSL2591_FULLSPECTRUM);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TSL2591::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;

	data["Vis"]  = vis;
  LMark;
	data["IR"]   = ir;
  LMark;
	data["Full"] = full;
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TSL2591::diagnose(bool& result){
  LMark;
	// implement here
}

///////////////////////////////////////////////////////////////////////////////
