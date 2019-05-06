
#include "Loom_TSL2591.h"

#include <Adafruit_Sensor.h>


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_TSL2591::Loom_TSL2591(
		byte			i2c_address, 
		const char*		module_name, 
		uint8_t			gain_level, 
		uint8_t			timing_level
	)
	: LoomI2CSensor( module_name, i2c_address )
{
	this->module_type = ModuleType::TSL2591;

	this->gain_level   = gain_level;
	this->timing_level = timing_level;

	inst_tsl2591 = Adafruit_TSL2591(i2c_address);

	bool setup = inst_tsl2591.begin();
		
	if (setup) {
		switch(gain_level) {
			case 0 : inst_tsl2591.setGain(TSL2591_GAIN_LOW);    break;  // 1x gain (bright light)
			case 1 : inst_tsl2591.setGain(TSL2591_GAIN_MED);    break;  // 25x gain
			case 2 : inst_tsl2591.setGain(TSL2591_GAIN_HIGH);   break;  // 428x gain
			case 3 : inst_tsl2591.setGain(TSL2591_GAIN_MAX);    break;  // 9876x gain
			default: Println("Invalid gain level."); break;
		}
	  
		switch(timing_level) {
			case 0 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_100MS); break; // shortest integration time (bright light)
			case 1 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_200MS); break;
			case 2 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_300MS); break;
			case 3 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_400MS); break;
			case 4 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_500MS); break;
			case 5 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light) break;
			default: Println("Invalid timing level"); break;
		}	
	} 

	print_module_label();
	Println2("Initialize ", (setup) ? "sucessful" : "failed");
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_TSL2591::Loom_TSL2591(JsonVariant p)
	: Loom_TSL2591(p[0], p[1], p[2], p[3])
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_TSL2591::~Loom_TSL2591() 
{

}

/////////////////////////////////////////////////////////////////////
void Loom_TSL2591::print_measurements()
{
	print_module_label();
	Println("Measurements:");
	Println3("\t", "Vis  : ", vis);
	Println3("\t", "IR   : ", ir);
	Println3("\t", "Full : ", full);
}

/////////////////////////////////////////////////////////////////////
void Loom_TSL2591::measure()
{
	vis  = inst_tsl2591.getLuminosity(TSL2591_VISIBLE);
	ir   = inst_tsl2591.getLuminosity(TSL2591_INFRARED);
	full = inst_tsl2591.getLuminosity(TSL2591_FULLSPECTRUM);
}

/////////////////////////////////////////////////////////////////////
void Loom_TSL2591::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);
	
	append_to_bundle(bndl, id_prefix, "Vis" , vis, NEW_MSG);
	append_to_bundle(bndl, id_prefix, "IR"  , ir);
	append_to_bundle(bndl, id_prefix, "Full", full);

}



