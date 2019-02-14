
#include "Loom_LIS3DH.h"




// --- CONSTRUCTOR ---
Loom_LIS3DH::Loom_LIS3DH(byte i2c_address, char* module_name, char* sensor_description)

	: LoomI2CSensor( module_name, sensor_description, i2c_address )
{
	inst_LIS3DH = new LIS3DH(I2C_MODE, i2c_address);

	inst_LIS3DH->settings.adcEnabled      = 1;
	inst_LIS3DH->settings.tempEnabled     = 1;
	inst_LIS3DH->settings.accelSampleRate = 50;  //Hz.  Can be: 0,1,10,25,50,100,200,400,1600,5000 Hz
	inst_LIS3DH->settings.accelRange      = 16;  //Max G force readable.  Can be: 2, 4, 8, 16
	inst_LIS3DH->settings.xAccelEnabled   = 1;
	inst_LIS3DH->settings.yAccelEnabled   = 1;
	inst_LIS3DH->settings.zAccelEnabled   = 1;
  
	bool setup = inst_LIS3DH->begin();

	print_module_label();
	LOOM_DEBUG_Println3("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}


// --- DESTRUCTOR ---
Loom_LIS3DH::~Loom_LIS3DH() 
{
	delete inst_LIS3DH;
}



void Loom_LIS3DH::print_measurements()
{
	print_module_label();
	LOOM_DEBUG_Println("Measurements:");
	LOOM_DEBUG_Println3("\t", "Accel X: ", accel[0]);
	LOOM_DEBUG_Println3("\t", "Accel Y: ", accel[1]);
	LOOM_DEBUG_Println3("\t", "Accel Z: ", accel[2]);
}


void Loom_LIS3DH::measure()
{
	accel[0] = inst_LIS3DH->readFloatAccelX();
	accel[1] = inst_LIS3DH->readFloatAccelY();
	accel[2] = inst_LIS3DH->readFloatAccelZ();
}


void Loom_LIS3DH::package(OSCBundle* bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	append_to_bundle_key_value(bndl, id_prefix, "Accel_X", accel[0]);
	append_to_bundle_key_value(bndl, id_prefix, "Accel_Y", accel[1]);
	append_to_bundle_key_value(bndl, id_prefix, "Accel_Z", accel[2]);
}


// void Loom_LIS3DH::package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port)
// {
// 	LoomI2CSensor::package_mux(bndl, id_prefix, port);

// 	append_to_bundle_msg_key_value(bndl, "Accel_X", accel[0]);
// 	append_to_bundle_msg_key_value(bndl, "Accel_Y", accel[1]);
// 	append_to_bundle_msg_key_value(bndl, "Accel_Z", accel[2]);
// }
