
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


void Loom_MS5803::package(OSCBundle* bndl)
{
	char id_prefix[30]; 
	resolve_package_prefix(id_prefix);

	append_to_bundle_key_value(bndl, id_prefix, "Pressure", pressure);
	append_to_bundle_key_value(bndl, id_prefix, "Temp"    , temp);
}


void Loom_MS5803::package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port)
{
	LoomI2CSensor::package_mux(bndl, id_prefix, port);

	append_to_bundle_key_value(bndl, id_prefix, "Pressure", pressure);
	append_to_bundle_key_value(bndl, id_prefix, "Temp"    , temp);
}
