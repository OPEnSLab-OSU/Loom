
#include "Loom_MS5803.h"



/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_MS5803::Loom_MS5803(
		byte			i2c_address, 
		const char*		module_name
	)
	: LoomI2CSensor( module_name, i2c_address )
{
	this->module_type = ModuleType::MS5803;

	inst_MS5803 = new MS_5803(i2c_address, 512);
	bool setup = inst_MS5803->initializeMS_5803();

	print_module_label();
	Println2("\tInitialize ", (setup) ? "sucessful" : "failed");
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_MS5803::Loom_MS5803(JsonVariant p)
	: Loom_MS5803(p[0], p[1]) 
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_MS5803::~Loom_MS5803() 
{
	delete inst_MS5803;
}

/////////////////////////////////////////////////////////////////////
void Loom_MS5803::print_measurements()
{
	print_module_label();
	Println("Measurements:");
	Println4("\t", "Pressure    : ", pressure , " mbar");
	Println4("\t", "Temperature : ", temp     , " C");
}

/////////////////////////////////////////////////////////////////////
void Loom_MS5803::measure()
{
	inst_MS5803->readSensor();

	pressure = inst_MS5803->pressure();
	temp     = inst_MS5803->temperature();
}

/////////////////////////////////////////////////////////////////////
void Loom_MS5803::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	append_to_bundle(bndl, id_prefix, "Pressure", pressure, NEW_MSG);
	append_to_bundle(bndl, id_prefix, "Temp"    , temp);
}


