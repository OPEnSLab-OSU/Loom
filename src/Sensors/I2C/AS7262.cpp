///////////////////////////////////////////////////////////////////////////////
///
/// @file		AS7262.cpp
/// @brief		File for AS7262 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "AS7262.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
AS7262::AS7262(
		const byte			i2c_address,
		const uint8_t		mux_port,
		const bool			use_bulb,
		const uint8_t		gain,
		const uint8_t		mode,
		const uint8_t		integration_time
	)
	: I2CSensor("AS7262", i2c_address, mux_port)
	, use_bulb(use_bulb)
	, gain(gain)
	, mode(mode)
	, integration_time(integration_time)

{
  LMark;
	inst_AS7262.begin(Wire, gain, mode);
  LMark;
	inst_AS7262.setIntegrationTime(integration_time);

	print_module_label();
  LMark;
	LPrintln("Initialized");
}

///////////////////////////////////////////////////////////////////////////////
AS7262::AS7262(JsonArrayConst p)
	: AS7262(EXPAND_ARRAY(p, 6) ) {}

///////////////////////////////////////////////////////////////////////////////
void AS7262::print_config() const
{
	I2CSensor::print_config();
  LMark;
	LPrintln("\tGain     : ", gain);
  LMark;
	LPrintln("\tMode     : ", mode);
  LMark;
	LPrintln("\tUse Bulb : ", (use_bulb) ? "True" : "False");
}

///////////////////////////////////////////////////////////////////////////////
void AS7262::print_measurements() const
{
	print_module_label();
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
}

///////////////////////////////////////////////////////////////////////////////
void AS7262::measure()
{
  LMark;
	if (use_bulb) {
   	LMark;
		inst_AS7262.takeMeasurementsWithBulb();
	} else {
		LMark;
		inst_AS7262.takeMeasurements();
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
}

///////////////////////////////////////////////////////////////////////////////
void AS7262::package(JsonObject json)
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
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
