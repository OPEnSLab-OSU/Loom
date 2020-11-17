///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_AS7262.cpp
/// @brief		File for Loom_AS7262 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "AS7262.h"


///////////////////////////////////////////////////////////////////////////////
Loom_AS7262::Loom_AS7262(
LoomManager* manager,
const byte i2c_address,
		const uint8_t		mux_port,
		const bool			use_bulb,
		const uint8_t		gain,
		const uint8_t		mode,
		const uint8_t		integration_time
	)
	: LoomI2CSensor(manager, "AS7262", Type::AS7262, i2c_address, mux_port )
	, use_bulb(use_bulb)
	, gain(gain)
	, mode(mode)
	, integration_time(integration_time)

{
  LMark;
	inst_AS7262.begin(Wire, gain, mode);
  LMark;
	inst_AS7262.setIntegrationTime(integration_time);
  LMark;

	print_module_label();
  LMark;
	LPrintln("Initialized");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_AS7262::Loom_AS7262(LoomManager* manager, JsonArrayConst p)
	: Loom_AS7262(manager, EXPAND_ARRAY(p, 6) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7262::print_config() const
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
void Loom_AS7262::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Measurements:");
  LMark;
	LPrintln("\tViolet : ", color_vals[0]);
  LMark;
	LPrintln("\tBlue   : ", color_vals[1]);
  LMark;
	LPrintln("\tGreen  : ", color_vals[2]);
  LMark;
	LPrintln("\tYellow : ", color_vals[3]);
  LMark;
	LPrintln("\tOrange : ", color_vals[4]);
  LMark;
	LPrintln("\tRed    : ", color_vals[5]);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7262::measure()
{
  LMark;
	if (use_bulb) {
   	LMark;
		inst_AS7262.takeMeasurementsWithBulb();
  	LMark;
	} else {
		inst_AS7262.takeMeasurements();
  	LMark;
	}

	color_vals[0] = inst_AS7262.getViolet();
  LMark;
	color_vals[1] = inst_AS7262.getBlue();
  LMark;
	color_vals[2] = inst_AS7262.getGreen();
  LMark;
	color_vals[3] = inst_AS7262.getYellow();
  LMark;
	color_vals[4] = inst_AS7262.getOrange();
  LMark;
	color_vals[5] = inst_AS7262.getRed();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7262::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;
	data["Violet"]	= color_vals[0];
  LMark;
	data["Blue"]	= color_vals[1];
  LMark;
	data["Green"]	= color_vals[2];
  LMark;
	data["Yellow"]	= color_vals[3];
  LMark;
	data["Orange"]	= color_vals[4];
  LMark;
	data["Red"]		= color_vals[5];
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7262::diagnose(bool& result){
  LMark;
	// implement here
}

///////////////////////////////////////////////////////////////////////////////
