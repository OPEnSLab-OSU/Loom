
#include "Loom_FXAS21002.h"

#include <Adafruit_Sensor.h>


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_FXAS21002::Loom_FXAS21002(
		byte			i2c_address, 
		const char*		module_name
	)
	: LoomI2CSensor( module_name, i2c_address )
{
	this->module_type = ModuleType::FXAS21002;

	inst_FXAS21002 = new Adafruit_FXAS21002C(0x0021002C);
	bool setup = inst_FXAS21002->begin();

	print_module_label();
	LPrintln("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_FXAS21002::Loom_FXAS21002(JsonVariant p)
	: Loom_FXAS21002(p[0], p[1]) 
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_FXAS21002::~Loom_FXAS21002() 
{
	delete inst_FXAS21002;
}

/////////////////////////////////////////////////////////////////////
void Loom_FXAS21002::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "gx: ", gyro[0]);
	LPrintln("\t", "gy: ", gyro[1]);
	LPrintln("\t", "gz: ", gyro[2]);
}

/////////////////////////////////////////////////////////////////////
void Loom_FXAS21002::measure()
{
	sensors_event_t event;
	inst_FXAS21002->getEvent(&event);

	gyro[0] = event.gyro.x;
	gyro[1] = event.gyro.y;
	gyro[2] = event.gyro.z;
}

/////////////////////////////////////////////////////////////////////
void Loom_FXAS21002::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	append_to_bundle(bndl, id_prefix, "gx", gyro[0], NEW_MSG);
	append_to_bundle(bndl, id_prefix, "gy", gyro[1]);
	append_to_bundle(bndl, id_prefix, "gz", gyro[2]);
}


