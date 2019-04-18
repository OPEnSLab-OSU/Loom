
#include "Loom_Sensor.h"

/////////////////////////////////////////////////////////////////////
LoomSensor::LoomSensor(	
		char*		module_name,
		uint8_t		num_samples 
	) 
	: LoomModule( module_name ) 
{
	this->num_samples        = num_samples;
}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
LoomSensor::~LoomSensor() {}

/////////////////////////////////////////////////////////////////////
void LoomSensor::print_config()
{
	LoomModule::print_config();

	Println3('\t', "Num Samples         : ", num_samples);
}


/////////////////////////////////////////////////////////////////////
void LoomSensor::set_num_samples(uint8_t n) 
{ 
	num_samples = n; 
}

/////////////////////////////////////////////////////////////////////
uint8_t LoomSensor::get_num_samples() 
{ 
	return num_samples; 
}


