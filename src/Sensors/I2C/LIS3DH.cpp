///////////////////////////////////////////////////////////////////////////////
///
/// @file		LIS3DH.cpp
/// @brief		File for LIS3DH implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "LIS3DH.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
Loom::LIS3DH::LIS3DH(
		const byte		i2c_address,
		const uint8_t	mux_port
	)
	: I2CSensor("LIS3DH", i2c_address, mux_port)
	, inst_LIS3DH( ::LIS3DH(I2C_MODE, i2c_address) )

{
	inst_LIS3DH.settings.adcEnabled      = 1;
	inst_LIS3DH.settings.tempEnabled     = 1;
	inst_LIS3DH.settings.accelSampleRate = 50;  //Hz.  Can be: 0,1,10,25,50,100,200,400,1600,5000 Hz
	inst_LIS3DH.settings.accelRange      = 16;  //Max G force readable.  Can be: 2, 4, 8, 16
	inst_LIS3DH.settings.xAccelEnabled   = 1;
	inst_LIS3DH.settings.yAccelEnabled   = 1;
	inst_LIS3DH.settings.zAccelEnabled   = 1;
  LMark;

	bool setup = inst_LIS3DH.begin();
  LMark;

	if (!setup) active = false;

	print_module_label();
  LMark;
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom::LIS3DH::LIS3DH(JsonArrayConst p)
	: LIS3DH(EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom::LIS3DH::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
  LMark;
	LPrintln("\tAccel X: ", accel[0]);
  LMark;
	LPrintln("\tAccel Y: ", accel[1]);
  LMark;
	LPrintln("\tAccel Z: ", accel[2]);
}

///////////////////////////////////////////////////////////////////////////////
void Loom::LIS3DH::measure()
{
  LMark;
	accel[0] = inst_LIS3DH.readFloatAccelX();
  LMark;
	accel[1] = inst_LIS3DH.readFloatAccelY();
  LMark;
	accel[2] = inst_LIS3DH.readFloatAccelZ();
}

///////////////////////////////////////////////////////////////////////////////
void Loom::LIS3DH::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;

	data["ax"] = accel[0];
  LMark;
	data["ay"] = accel[1];
  LMark;
	data["az"] = accel[2];
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
