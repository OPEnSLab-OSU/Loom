
#include "Loom_AS7263.h"


///////////////////////////////////////////////////////////////////////////////
Loom_AS7263::Loom_AS7263(
		byte			i2c_address, 
		uint8_t			mux_port,
		bool			use_bulb, 
		uint8_t			gain, 
		uint8_t			mode, 
		uint8_t			integration_time
	)
	: LoomI2CSensor( "AS7263", Type::AS7263, i2c_address, mux_port )
	, use_bulb(use_bulb)
	, gain(gain)
	, mode(mode)
	, integration_time(integration_time)
{
	inst_AS7263.begin(Wire, gain, mode);
	inst_AS7263.setIntegrationTime(integration_time);

	print_module_label();
	LPrintln("Initialized");
}

///////////////////////////////////////////////////////////////////////////////
Loom_AS7263::Loom_AS7263(JsonArrayConst p)
	: Loom_AS7263( EXPAND_ARRAY(p, 6) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::print_config()
{
	LoomI2CSensor::print_config();	
	LPrintln("\tGain     : ", gain);
	LPrintln("\tMode     : ", mode);
	LPrintln("\tUse Bulb : ", (use_bulb) ? "True" : "False");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tNIR R: ", nir_vals[0]);
	LPrintln("\tNIR S: ", nir_vals[1]);
	LPrintln("\tNIR T: ", nir_vals[2]);
	LPrintln("\tNIR U: ", nir_vals[3]);
	LPrintln("\tNIR V: ", nir_vals[4]);
	LPrintln("\tNIR W: ", nir_vals[5]);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::measure()
{
	if (use_bulb) {
		inst_AS7263.takeMeasurementsWithBulb();
	} else {
		inst_AS7263.takeMeasurements();
	}

	nir_vals[0] = inst_AS7263.getR();
	nir_vals[1] = inst_AS7263.getS();
	nir_vals[2] = inst_AS7263.getT();
	nir_vals[3] = inst_AS7263.getU();
	nir_vals[4] = inst_AS7263.getV();
	nir_vals[5] = inst_AS7263.getW();	
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	data["NIR_R"] = nir_vals[0];
	data["NIR_S"] = nir_vals[1];
	data["NIR_T"] = nir_vals[2];
	data["NIR_U"] = nir_vals[3];
	data["NIR_V"] = nir_vals[4];
	data["NIR_W"] = nir_vals[5];

}

///////////////////////////////////////////////////////////////////////////////

