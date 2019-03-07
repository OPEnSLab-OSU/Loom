
#include "Loom_SPI_Sensor.h"

/////////////////////////////////////////////////////////////////////
LoomSPISensor::LoomSPISensor(	char* 	module_name,
								char* 	sensor_description, 
								uint8_t	num_samples 

						  ) : LoomSensor( module_name, sensor_description, num_samples ) 
{



}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
LoomSPISensor::~LoomSPISensor() {}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void LoomSPISensor::print_config()
{
	LoomSensor::print_config();
}


