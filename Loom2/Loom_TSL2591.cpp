
#include "Loom_TSL2591.h"

#include <Wire.h>




// --- CONSTRUCTOR ---
// Loom_TSL2591::Loom_TSL2591(char* module_name, char* sensor_description, byte i2c_address)
Loom_TSL2591::Loom_TSL2591(byte i2c_address, char* module_name, char* sensor_description, uint8_t gain_level, uint8_t timing_level)

	: LoomI2CSensor( module_name, sensor_description, i2c_address )
{
	// LOOM_DEBUG_Println("LIS3DH Setup");

	this->gain_level   = gain_level;
	this->timing_level = timing_level;

	inst_tsl2591 = Adafruit_TSL2591(i2c_address);

	bool setup = inst_tsl2591.begin();
		
	if (setup) {
		configure_settings(); //Medium gain and timing
	} 

	LOOM_DEBUG_Println4("\tInitialize ", module_name, " ", (setup) ? "sucessful" : "failed");

}

// maybe a constructor that specifies the i2c address (use a default otherwise)

// --- DESTRUCTOR ---
Loom_TSL2591::~Loom_TSL2591() 
{
	// delete inst_tsl2591;
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
	LOOM_DEBUG_Println2("\tVis  : ", vis);
	LOOM_DEBUG_Println2("\tIR   : ", ir);
	LOOM_DEBUG_Println2("\tFull : ", full);
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
	
	append_to_bundle_key_value(bndl, id_prefix, "Vis", vis);
	append_to_bundle_key_value(bndl, id_prefix, "IR", ir);
	append_to_bundle_key_value(bndl, id_prefix, "Full", full);
}

void Loom_TSL2591::package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port)
{
	LoomI2CSensor::package_mux(bndl, id_prefix, port);

	append_to_bundle_msg_key_value(bndl, "Vis", vis);
	append_to_bundle_msg_key_value(bndl, "IR", ir);
	append_to_bundle_msg_key_value(bndl, "Full", full);
}

void Loom_TSL2591::configure_settings() 
{
	switch(gain_level) {
		case 0 : inst_tsl2591.setGain(TSL2591_GAIN_LOW);  break;  // 1x gain (bright light)
		case 1 : inst_tsl2591.setGain(TSL2591_GAIN_MED);  break;  // 25x gain
		case 2 : inst_tsl2591.setGain(TSL2591_GAIN_HIGH); break;  // 428x gain
		case 3 : inst_tsl2591.setGain(TSL2591_GAIN_MAX);  break;  // 9876x gain
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
	
	// #if LOOM_DEBUG == 1
	// 	/* Display the gain and integration time for reference sake */  
	// 	Serial.println(F("------------------------------------"));
	// 	Serial.print  (F("Gain:         "));
	// 	tsl2591Gain_t gain = inst_tsl2591.getGain();
	// 	switch(gain)
	// 	{
	// 		case TSL2591_GAIN_LOW:  Serial.println(F("1x (Low)")); 		break;
	// 		case TSL2591_GAIN_MED:  Serial.println(F("25x (Medium)")); 	break;
	// 		case TSL2591_GAIN_HIGH: Serial.println(F("428x (High)")); 	break;
	// 		case TSL2591_GAIN_MAX:  Serial.println(F("9876x (Max)")); 	break;
	// 	}
	// 	Serial.print  (F("Timing:       "));
	// 	Serial.print((inst_tsl2591.getTiming() + 1) * 100, DEC); 
	// 	Serial.println(F(" ms"));
	// 	Serial.println(F("------------------------------------"));
	// 	Serial.println(F(""));
	// #endif //if LOOM_DEBUG == 1
}
