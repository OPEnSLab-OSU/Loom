
#include "Loom_MS5803.h"




// --- CONSTRUCTOR ---
Loom_MS5803::Loom_MS5803(byte i2c_address, char* module_name, char* sensor_description)

	: LoomI2CSensor( module_name, sensor_description, i2c_address )
{
	inst_MS5803 = new MS_5803(512);
	bool setup = inst_MS5803->initializeMS_5803();

	print_module_label();
	LOOM_DEBUG_Println2("\tInitialize ", (setup) ? "sucessful" : "failed");
}


// --- DESTRUCTOR ---
Loom_MS5803::~Loom_MS5803() 
{
	delete inst_MS5803;
}



void Loom_MS5803::print_measurements()
{
	print_module_label();
	LOOM_DEBUG_Println("Measurements:");
	LOOM_DEBUG_Println4("\t", "Pressure    : ", pressure , " mbar");
	LOOM_DEBUG_Println4("\t", "Temperature : ", temp     , " C");
}


void Loom_MS5803::measure()
{
	inst_MS5803->readSensor();

	pressure = inst_MS5803->pressure();
	temp     = inst_MS5803->temperature();
}


void Loom_MS5803::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	append_to_bundle(bndl, id_prefix, "Pressure", pressure, NEW_MSG);
	append_to_bundle(bndl, id_prefix, "Temp"    , temp);
}


