
#include "Loom_Decagon_5TM.h"


Loom_Decagon5TM::Loom_Decagon5TM(	char* 	module_name,
									char* 	sensor_description, 
									uint8_t	num_samples 

						  ) : LoomSDI12Sensor( module_name, sensor_description, num_samples ) 
{



}


// --- DESTRUCTOR ---
Loom_Decagon5TM::~Loom_Decagon5TM() {}


// --- PUBLIC METHODS ---

void Loom_Decagon5TM::print_config()
{
	LoomSDI12Sensor::print_config();

}

void Loom_Decagon5TM::print_measurements()
{

}

void Loom_Decagon5TM::measure()
{

}

void Loom_Decagon5TM::package(OSCBundle& bndl, char* suffix)
{

}

