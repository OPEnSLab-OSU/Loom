
#include "Loom_FXAS21002.h"



// --- CONSTRUCTOR ---
Loom_FXAS21002::Loom_FXAS21002(byte i2c_address, char* module_name, char* sensor_description)

	: LoomI2CSensor( module_name, sensor_description, i2c_address )
{
	inst_FXAS21002 = new Adafruit_FXAS21002C(0x0021002C);
	bool setup = inst_FXAS21002->begin();

	print_module_label();
	LOOM_DEBUG_Println3("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}


// --- DESTRUCTOR ---
Loom_FXAS21002::~Loom_FXAS21002() 
{

}



void Loom_FXAS21002::print_measurements()
{
	print_module_label();
	LOOM_DEBUG_Println("Measurements:");
	LOOM_DEBUG_Println3("\t", "gx: ", gyro[0]);
	LOOM_DEBUG_Println3("\t", "gy: ", gyro[1]);
	LOOM_DEBUG_Println3("\t", "gz: ", gyro[2]);
}


void Loom_FXAS21002::measure()
{
	sensors_event_t event;
	inst_FXAS21002->getEvent(&event);

	gyro[0] = event.gyro.x;
	gyro[1] = event.gyro.y;
	gyro[2] = event.gyro.z;
}


void Loom_FXAS21002::package(OSCBundle* bndl)
{
	char id_prefix[30]; 
	resolve_package_prefix(id_prefix);

	append_to_bundle_key_value(bndl, id_prefix, "gx", gyro[0]);
	append_to_bundle_key_value(bndl, id_prefix, "gy", gyro[1]);
	append_to_bundle_key_value(bndl, id_prefix, "gz", gyro[2]);
}


void Loom_FXAS21002::package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port)
{
	LoomI2CSensor::package_mux(bndl, id_prefix, port);

	append_to_bundle_msg_key_value(bndl, "gx", gyro[0]);
	append_to_bundle_msg_key_value(bndl, "gy", gyro[1]);
	append_to_bundle_msg_key_value(bndl, "gz", gyro[2]);
}
