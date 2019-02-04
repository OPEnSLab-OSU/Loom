
#include "Loom_TSL2591.h"




// --- CONSTRUCTOR ---
// Loom_TSL2591::Loom_TSL2591(char* module_name, char* sensor_description, byte i2c_address)
Loom_TSL2591::Loom_TSL2591(byte i2c_address, char* module_name, char* sensor_description, uint8_t gain_level, uint8_t timing_level)

	: LoomI2CSensor( module_name, sensor_description, i2c_address )
{
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
			default: LOOM_DEBUG_Println("Invalid gain level."); break;
		}
	  
		switch(timing_level) {
			case 0 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_100MS); break; // shortest integration time (bright light)
			case 1 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_200MS); break;
			case 2 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_300MS); break;
			case 3 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_400MS); break;
			case 4 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_500MS); break;
			case 5 : inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light) break;
			default: LOOM_DEBUG_Println("Invalid timing level"); break;
		}	
	} 

	print_module_label();
	LOOM_DEBUG_Println3("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}

// maybe a constructor that specifies the i2c address (use a default otherwise)

// --- DESTRUCTOR ---
Loom_TSL2591::~Loom_TSL2591() 
{

}


// --- PUBLIC METHODS ---

// void Loom_TSL2591::print_config()
// {
// 	I2CSensor::print_config();
// }


void Loom_TSL2591::print_measurements()
{
	print_module_label();
	LOOM_DEBUG_Println("Measurements:");
	LOOM_DEBUG_Println3("\t", "Vis  : ", vis);
	LOOM_DEBUG_Println3("\t", "IR   : ", ir);
	LOOM_DEBUG_Println3("\t", "Full : ", full);
}


void Loom_TSL2591::measure()
{
	vis  = inst_tsl2591.getLuminosity(TSL2591_VISIBLE);
	ir   = inst_tsl2591.getLuminosity(TSL2591_INFRARED);
	full = inst_tsl2591.getLuminosity(TSL2591_FULLSPECTRUM);
}


void Loom_TSL2591::package(OSCBundle* bndl)
{
	char id_prefix[30]; 
	resolve_package_prefix(id_prefix);
	
	append_to_bundle_key_value(bndl, id_prefix, "Vis" , vis);
	append_to_bundle_key_value(bndl, id_prefix, "IR"  , ir);
	append_to_bundle_key_value(bndl, id_prefix, "Full", full);
}


void Loom_TSL2591::package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port)
{
	LoomI2CSensor::package_mux(bndl, id_prefix, port);

	append_to_bundle_msg_key_value(bndl, "Vis" , vis);
	append_to_bundle_msg_key_value(bndl, "IR"  , ir);
	append_to_bundle_msg_key_value(bndl, "Full", full);
}
