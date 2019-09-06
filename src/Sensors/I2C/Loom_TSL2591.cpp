
#include "Loom_TSL2591.h"

#include <Adafruit_Sensor.h>


///////////////////////////////////////////////////////////////////////////////
Loom_TSL2591::Loom_TSL2591(
		byte			i2c_address, 
		uint8_t			mux_port,
		uint8_t			gain_level, 
		uint8_t			timing_level
	)
	: LoomI2CSensor( "TSL2591", Type::TSL2591, i2c_address, mux_port )
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
    
    for(int i = 0; i < 3; i++) {
        Values.push_back(var());
    }

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_TSL2591::Loom_TSL2591(JsonArrayConst p)
	: Loom_TSL2591( EXPAND_ARRAY(p, 4) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_TSL2591::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tVis  : ", Values[0].retrieve<uint16_t>().value_or(0));
	LPrintln("\tIR   : ", Values[1].retrieve<uint16_t>().value_or(0));
	LPrintln("\tFull : ", Values[2].retrieve<uint16_t>().value_or(0));
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TSL2591::measure()
{
	Values[0] = inst_tsl2591.getLuminosity(TSL2591_VISIBLE); //< Vis
	Values[1] = inst_tsl2591.getLuminosity(TSL2591_INFRARED); //< Ir
	Values[2] = inst_tsl2591.getLuminosity(TSL2591_FULLSPECTRUM); //< Full
}

///////////////////////////////////////////////////////////////////////////////
void Loom_TSL2591::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["Vis"]  = Values[0].retrieve<uint16_t>().value_or(0);
	data["IR"]   = Values[1].retrieve<uint16_t>().value_or(0);
	data["Full"] = Values[2].retrieve<uint16_t>().value_or(0);
}

///////////////////////////////////////////////////////////////////////////////


