
#include "Loom_LIS3DH.h"


///////////////////////////////////////////////////////////////////////////////
Loom_LIS3DH::Loom_LIS3DH(
		byte			i2c_address, 
		int				mux_port,
		const char*		module_name
	) : LoomI2CSensor( module_name, i2c_address, mux_port )
{
	this->module_type = LoomModule::Type::LIS3DH;

	inst_LIS3DH = new LIS3DH(I2C_MODE, i2c_address);

	inst_LIS3DH->settings.adcEnabled      = 1;
	inst_LIS3DH->settings.tempEnabled     = 1;
	inst_LIS3DH->settings.accelSampleRate = 50;  //Hz.  Can be: 0,1,10,25,50,100,200,400,1600,5000 Hz
	inst_LIS3DH->settings.accelRange      = 16;  //Max G force readable.  Can be: 2, 4, 8, 16
	inst_LIS3DH->settings.xAccelEnabled   = 1;
	inst_LIS3DH->settings.yAccelEnabled   = 1;
	inst_LIS3DH->settings.zAccelEnabled   = 1;
  
	bool setup = inst_LIS3DH->begin();

	if (!setup) active = false;

	print_module_label();
	LPrintln("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_LIS3DH::Loom_LIS3DH(JsonArrayConst p)
	: Loom_LIS3DH( EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
Loom_LIS3DH::~Loom_LIS3DH() 
{
	delete inst_LIS3DH;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LIS3DH::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Accel X: ", accel[0]);
	LPrintln("\t", "Accel Y: ", accel[1]);
	LPrintln("\t", "Accel Z: ", accel[2]);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LIS3DH::measure()
{
	accel[0] = inst_LIS3DH->readFloatAccelX();
	accel[1] = inst_LIS3DH->readFloatAccelY();
	accel[2] = inst_LIS3DH->readFloatAccelZ();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LIS3DH::package(JsonObject json)
{
	package_json(json, module_name, 
		"ax", accel[0],
		"ay", accel[1],
		"az", accel[2]
	);
}

///////////////////////////////////////////////////////////////////////////////



