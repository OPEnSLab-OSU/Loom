///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_LIS3DH.cpp
/// @brief		File for Loom_LIS3DH implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "LIS3DH.h"


///////////////////////////////////////////////////////////////////////////////
Loom_LIS3DH::Loom_LIS3DH(
LoomManager* manager,
const byte i2c_address,
		const uint8_t		mux_port
	)
	: LoomI2CSensor(manager, "LIS3DH", Type::LIS3DH, i2c_address, mux_port )
	, inst_LIS3DH( LIS3DH(I2C_MODE, i2c_address) )

{
  LMark;
	inst_LIS3DH.settings.adcEnabled      = 1;
  LMark;
	inst_LIS3DH.settings.tempEnabled     = 1;
  LMark;
	inst_LIS3DH.settings.accelSampleRate = 50;  //Hz.  Can be: 0,1,10,25,50,100,200,400,1600,5000 Hz
  LMark;
	inst_LIS3DH.settings.accelRange      = 16;  //Max G force readable.  Can be: 2, 4, 8, 16
  LMark;
	inst_LIS3DH.settings.xAccelEnabled   = 1;
  LMark;
	inst_LIS3DH.settings.yAccelEnabled   = 1;
  LMark;
	inst_LIS3DH.settings.zAccelEnabled   = 1;
  LMark;

	bool setup = inst_LIS3DH.begin();
  LMark;

	if (!setup) active = false;
  LMark;

	print_module_label();
  LMark;
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_LIS3DH::Loom_LIS3DH(LoomManager* manager, JsonArrayConst p)
	: Loom_LIS3DH(manager, EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_LIS3DH::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Measurements:");
  LMark;
	LPrintln("\tAccel X: ", accel[0]);
  LMark;
	LPrintln("\tAccel Y: ", accel[1]);
  LMark;
	LPrintln("\tAccel Z: ", accel[2]);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LIS3DH::measure()
{
  LMark;
	accel[0] = inst_LIS3DH.readFloatAccelX();
  LMark;
	accel[1] = inst_LIS3DH.readFloatAccelY();
  LMark;
	accel[2] = inst_LIS3DH.readFloatAccelZ();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LIS3DH::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;

	data["ax"] = accel[0];
  LMark;
	data["ay"] = accel[1];
  LMark;
	data["az"] = accel[2];
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LIS3DH::diagnose(bool& result){
  LMark;
	// implement here
}

///////////////////////////////////////////////////////////////////////////////
