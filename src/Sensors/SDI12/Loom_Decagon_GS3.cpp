
#include "Loom_Decagon_GS3.h"


Loom_DecagonGS3::Loom_DecagonGS3(	char* 	module_name,
									char* 	sensor_description, 
									uint8_t	num_samples 

						  ) : LoomSDI12Sensor( module_name, sensor_description, num_samples ) 
{



}


// --- DESTRUCTOR ---
Loom_DecagonGS3::~Loom_DecagonGS3() {}


// --- PUBLIC METHODS ---

void Loom_DecagonGS3::print_config()
{
	LoomSDI12Sensor::print_config();

}

void Loom_DecagonGS3::print_measurements()
{

}

void Loom_DecagonGS3::measure()
{

}

void Loom_DecagonGS3::package(OSCBundle& bndl, char* suffix)
{

}

