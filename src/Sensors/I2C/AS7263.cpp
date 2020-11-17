///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_AS7263.cpp
/// @brief		File for Loom_AS7263 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "AS7263.h"


///////////////////////////////////////////////////////////////////////////////
Loom_AS7263::Loom_AS7263(
LoomManager* manager,
const byte i2c_address,
		const uint8_t		mux_port,
		const bool			use_bulb,
		const uint8_t		gain,
		const uint8_t		mode,
		const uint8_t		integration_time
	)
	: LoomI2CSensor(manager, "AS7263", Type::AS7263, i2c_address, mux_port )
	, use_bulb(use_bulb)
	, gain(gain)
	, mode(mode)
	, integration_time(integration_time)
{
  LMark;
	inst_AS7263.begin(Wire, gain, mode);
  LMark;
	inst_AS7263.setIntegrationTime(integration_time);
  LMark;

	print_module_label();
  LMark;
	LPrintln("Initialized");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_AS7263::Loom_AS7263(LoomManager* manager, JsonArrayConst p)
	: Loom_AS7263(manager, EXPAND_ARRAY(p, 6) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::print_config() const
{
  LMark;
	LoomI2CSensor::print_config();
  LMark;
	LPrintln("\tGain     : ", gain);
  LMark;
	LPrintln("\tMode     : ", mode);
  LMark;
	LPrintln("\tUse Bulb : ", (use_bulb) ? "True" : "False");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Measurements:");
  LMark;
	LPrintln("\tNIR R: ", nir_vals[0]);
  LMark;
	LPrintln("\tNIR S: ", nir_vals[1]);
  LMark;
	LPrintln("\tNIR T: ", nir_vals[2]);
  LMark;
	LPrintln("\tNIR U: ", nir_vals[3]);
  LMark;
	LPrintln("\tNIR V: ", nir_vals[4]);
  LMark;
	LPrintln("\tNIR W: ", nir_vals[5]);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::measure()
{
  LMark;
	if (use_bulb) {
   	LMark;
		inst_AS7263.takeMeasurementsWithBulb();
  	LMark;
	} else {
		inst_AS7263.takeMeasurements();
  	LMark;
	}

	nir_vals[0] = inst_AS7263.getR();
  LMark;
	nir_vals[1] = inst_AS7263.getS();
  LMark;
	nir_vals[2] = inst_AS7263.getT();
  LMark;
	nir_vals[3] = inst_AS7263.getU();
  LMark;
	nir_vals[4] = inst_AS7263.getV();
  LMark;
	nir_vals[5] = inst_AS7263.getW();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;
	data["NIR_R"] = nir_vals[0];
  LMark;
	data["NIR_S"] = nir_vals[1];
  LMark;
	data["NIR_T"] = nir_vals[2];
  LMark;
	data["NIR_U"] = nir_vals[3];
  LMark;
	data["NIR_V"] = nir_vals[4];
  LMark;
	data["NIR_W"] = nir_vals[5];
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::diagnose(bool& result){
  LMark;
	// implement here
}

///////////////////////////////////////////////////////////////////////////////
