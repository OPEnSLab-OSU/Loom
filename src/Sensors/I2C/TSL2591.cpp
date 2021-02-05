///////////////////////////////////////////////////////////////////////////////
///
/// @file		TSL2591.cpp
/// @brief		File for TSL2591 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "TSL2591.h"
#include "Module_Factory.h"

#include <Adafruit_Sensor.h>

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
TSL2591::TSL2591(
		const byte		i2c_address,
		const uint8_t	mux_port,
		const uint8_t	gain_level,
		const uint8_t	timing_level
	)
	: I2CSensor("TSL2591", i2c_address, mux_port)
	, gain_level(gain_level)
	, timing_level(timing_level)
	, inst_tsl2591( Adafruit_TSL2591(i2c_address) )
{
  LMark;
	bool setup = inst_tsl2591.begin();
  LMark;

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
  LMark;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
TSL2591::TSL2591(JsonArrayConst p)
	: TSL2591(EXPAND_ARRAY(p, 4) ) {}

///////////////////////////////////////////////////////////////////////////////
void TSL2591::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
  LMark;
	LPrintln("\tVis  : ", vis);
  LMark;
	LPrintln("\tIR   : ", ir);
  LMark;
	LPrintln("\tFull : ", full);
}

///////////////////////////////////////////////////////////////////////////////
void TSL2591::measure()
{
  LMark;
	vis  = inst_tsl2591.getLuminosity(TSL2591_VISIBLE);
  LMark;
	ir   = inst_tsl2591.getLuminosity(TSL2591_INFRARED);
  LMark;
	full = inst_tsl2591.getLuminosity(TSL2591_FULLSPECTRUM);
}

///////////////////////////////////////////////////////////////////////////////
void TSL2591::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;

	data["Vis"]  = vis;
  LMark;
	data["IR"]   = ir;
  LMark;
	data["Full"] = full;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
