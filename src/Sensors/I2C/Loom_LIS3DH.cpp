
#include "Loom_LIS3DH.h"


///////////////////////////////////////////////////////////////////////////////
Loom_LIS3DH::Loom_LIS3DH(
		byte			i2c_address, 
		uint8_t			mux_port
	) 
	: LoomI2CSensor( "LIS3DH", Type::LIS3DH, i2c_address, mux_port )
	, inst_LIS3DH( LIS3DH(I2C_MODE, i2c_address) )
	
{
	inst_LIS3DH.settings.adcEnabled      = 1;
	inst_LIS3DH.settings.tempEnabled     = 1;
	inst_LIS3DH.settings.accelSampleRate = 50;  //Hz.  Can be: 0,1,10,25,50,100,200,400,1600,5000 Hz
	inst_LIS3DH.settings.accelRange      = 16;  //Max G force readable.  Can be: 2, 4, 8, 16
	inst_LIS3DH.settings.xAccelEnabled   = 1;
	inst_LIS3DH.settings.yAccelEnabled   = 1;
	inst_LIS3DH.settings.zAccelEnabled   = 1;
  
	bool setup = inst_LIS3DH.begin();

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_LIS3DH::Loom_LIS3DH(JsonArrayConst p)
	: Loom_LIS3DH( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_LIS3DH::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tAccel X: ", accel[0]);
	LPrintln("\tAccel Y: ", accel[1]);
	LPrintln("\tAccel Z: ", accel[2]);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LIS3DH::measure()
{
	accel[0] = inst_LIS3DH.readFloatAccelX();
	accel[1] = inst_LIS3DH.readFloatAccelY();
	accel[2] = inst_LIS3DH.readFloatAccelZ();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LIS3DH::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["ax"] = accel[0];
	data["ay"] = accel[1];
	data["az"] = accel[2];
}

///////////////////////////////////////////////////////////////////////////////



