///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_AS7262.cpp
/// @brief		File for Loom_AS7262 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Loom_AS7262.h"

///////////////////////////////////////////////////////////////////////////////
Loom_AS7262::Loom_AS7262(
		byte			i2c_address, 
		uint8_t			mux_port,
		bool			use_bulb, 
		uint8_t			gain, 
		uint8_t			mode, 
		uint8_t			integration_time
	)
	: LoomI2CSensor( "AS7262", Type::AS7262, i2c_address, mux_port )
	, use_bulb(use_bulb)
	, gain(gain)
	, mode(mode)
	, integration_time(integration_time)

{
	inst_AS7262.begin(Wire, gain, mode);
	inst_AS7262.setIntegrationTime(integration_time);
    
    for (int i = 0; i < 6; i++) {
        Values.push_back(var());
    }

	print_module_label();
	LPrintln("Initialized");
}

///////////////////////////////////////////////////////////////////////////////
Loom_AS7262::Loom_AS7262(JsonArrayConst p)
	: Loom_AS7262( EXPAND_ARRAY(p, 6) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7262::print_config()
{
	LoomI2CSensor::print_config();	
	LPrintln("\tGain     : ", gain);
	LPrintln("\tMode     : ", mode);
	LPrintln("\tUse Bulb : ", (use_bulb) ? "True" : "False");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7262::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tViolet : ", Values[0].retrieve<uint16_t>().value_or(0));
	LPrintln("\tBlue   : ", Values[1].retrieve<uint16_t>().value_or(0));
	LPrintln("\tGreen  : ", Values[2].retrieve<uint16_t>().value_or(0));
	LPrintln("\tYellow : ", Values[3].retrieve<uint16_t>().value_or(0));
	LPrintln("\tOrange : ", Values[4].retrieve<uint16_t>().value_or(0));
	LPrintln("\tRed    : ", Values[5].retrieve<uint16_t>().value_or(0));
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7262::measure()
{
	if (use_bulb) {
		inst_AS7262.takeMeasurementsWithBulb();
	} else {
		inst_AS7262.takeMeasurements();
	}

	Values[0] = inst_AS7262.getViolet();
	Values[1] = inst_AS7262.getBlue();
	Values[2] = inst_AS7262.getGreen();
	Values[3] = inst_AS7262.getYellow();
	Values[4] = inst_AS7262.getOrange();
	Values[5] = inst_AS7262.getRed();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7262::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	data["Violet"]	= Values[0].retrieve<uint16_t>().value_or(0);
	data["Blue"]	= Values[1].retrieve<uint16_t>().value_or(0);
	data["Green"]	= Values[2].retrieve<uint16_t>().value_or(0);
	data["Yellow"]	= Values[3].retrieve<uint16_t>().value_or(0);
	data["Orange"]	= Values[4].retrieve<uint16_t>().value_or(0);
    data["Red"]		= Values[5].retrieve<uint16_t>().value_or(0);
}

///////////////////////////////////////////////////////////////////////////////
