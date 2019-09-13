///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_AS7263.cpp
/// @brief		File for Loom_AS7263 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "Loom_AS7263.h"


///////////////////////////////////////////////////////////////////////////////
Loom_AS7263::Loom_AS7263(
		const byte			i2c_address, 
		const uint8_t		mux_port,
		const bool			use_bulb, 
		const uint8_t		gain, 
		const uint8_t		mode, 
		const uint8_t		integration_time
	)
	: LoomI2CSensor( "AS7263", Type::AS7263, i2c_address, mux_port )
	, use_bulb(use_bulb)
	, gain(gain)
	, mode(mode)
	, integration_time(integration_time)
{
	inst_AS7263.begin(Wire, gain, mode);
	inst_AS7263.setIntegrationTime(integration_time);
    
    for(int i = 0; i < 6; i++) {
        Values.push_back(var());
    }

	print_module_label();
	LPrintln("Initialized");
}

///////////////////////////////////////////////////////////////////////////////
Loom_AS7263::Loom_AS7263(JsonArrayConst p)
	: Loom_AS7263( EXPAND_ARRAY(p, 6) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::print_config() const
{
	LoomI2CSensor::print_config();	
	LPrintln("\tGain     : ", gain);
	LPrintln("\tMode     : ", mode);
	LPrintln("\tUse Bulb : ", (use_bulb) ? "True" : "False");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tNIR R: ", Values[0].retrieve<uint16_t>().value_or(0));
	LPrintln("\tNIR S: ", Values[1].retrieve<uint16_t>().value_or(0));
	LPrintln("\tNIR T: ", Values[2].retrieve<uint16_t>().value_or(0));
	LPrintln("\tNIR U: ", Values[3].retrieve<uint16_t>().value_or(0));
	LPrintln("\tNIR V: ", Values[4].retrieve<uint16_t>().value_or(0));
	LPrintln("\tNIR W: ", Values[5].retrieve<uint16_t>().value_or(0));
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::measure()
{
	if (use_bulb) {
		inst_AS7263.takeMeasurementsWithBulb();
	} else {
		inst_AS7263.takeMeasurements();
	}

	Values[0] = inst_AS7263.getR();
	Values[1] = inst_AS7263.getS();
	Values[2] = inst_AS7263.getT();
	Values[3] = inst_AS7263.getU();
	Values[4] = inst_AS7263.getV();
	Values[5] = inst_AS7263.getW();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	data["NIR_R"] = Values[0].retrieve<uint16_t>().value_or(0);
	data["NIR_S"] = Values[1].retrieve<uint16_t>().value_or(0);
	data["NIR_T"] = Values[2].retrieve<uint16_t>().value_or(0);
	data["NIR_U"] = Values[3].retrieve<uint16_t>().value_or(0);
	data["NIR_V"] = Values[4].retrieve<uint16_t>().value_or(0);
	data["NIR_W"] = Values[5].retrieve<uint16_t>().value_or(0);

}

///////////////////////////////////////////////////////////////////////////////

