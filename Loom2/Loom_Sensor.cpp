
#include "Loom_Sensor.h"
// #include "Loom_Device.h"

// #include "Loom_Macros.h"
// #include "Loom_Misc.h"
// #include "Loom_Translator.h"



LoomSensor::LoomSensor(	char* 	module_name,
	 
						char* 	sensor_description, 
						uint8_t	num_samples 

					  ) : LoomModule( module_name ) 
{
	this->sensor_description = sensor_description; 
	this->num_samples        = num_samples;
}


// --- DESTRUCTOR ---
LoomSensor::~LoomSensor() {}


// --- PUBLIC METHODS ---

void LoomSensor::print_config()
{
	LoomModule::print_config();

	LOOM_DEBUG_Println3('\t', "Description         : ", sensor_description);
	LOOM_DEBUG_Println3('\t', "Num Samples         : ", num_samples);
}



// void LoomSensor::calibrate() {}


// bool LoomSensor::message_route(OSCMessage* msg, int address_offset) {}


void LoomSensor::set_num_samples(uint8_t n) { num_samples = n; }


uint8_t LoomSensor::get_num_samples() { return num_samples; }


