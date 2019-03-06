
#include "Loom_SDI12_Sensor.h"


LoomSDI12Sensor::LoomSDI12Sensor(	char* 	module_name,
									char* 	sensor_description, 
									uint8_t	num_samples 

						  ) : LoomSensor( module_name, sensor_description, num_samples ) 
{



}


// --- DESTRUCTOR ---
LoomSDI12Sensor::~LoomSDI12Sensor() {}


// --- PUBLIC METHODS ---

void LoomSDI12Sensor::print_config()
{
	LoomSensor::print_config();

}


