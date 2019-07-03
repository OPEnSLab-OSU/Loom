
#include "Loom_SDI12_Sensor.h"

/////////////////////////////////////////////////////////////////////
LoomSDI12Sensor::LoomSDI12Sensor(	
		const char*		module_name,
		uint8_t			num_samples 
	) 
	: LoomSensor( module_name, num_samples ) 
{



}

/////////////////////////////////////////////////////////////////////
/// Destructor
LoomSDI12Sensor::~LoomSDI12Sensor() 
{
	
}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void LoomSDI12Sensor::print_config()
{
	LoomSensor::print_config();
}


