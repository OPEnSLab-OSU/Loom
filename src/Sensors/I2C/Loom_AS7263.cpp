
#include "Loom_AS7263.h"


///////////////////////////////////////////////////////////////////////////////
Loom_AS7263::Loom_AS7263(
		byte			i2c_address, 
		const char*		module_name,
		bool			use_bulb, 
		byte			gain, 
		byte			mode, 
		byte			integration_time
	)
	: LoomI2CSensor( module_name, i2c_address )
{
	this->module_type = LoomModule::Type::AS7263;

	this->use_bulb 			= use_bulb;
	this->gain 				= gain;
	this->mode 				= mode;
	this->integration_time 	= integration_time;

	inst_AS7263.begin(Wire, gain, mode);
	inst_AS7263.setIntegrationTime(integration_time);

	print_module_label();
	LPrintln("\t", "Initialized");
	// LPrintln("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_AS7263::Loom_AS7263(JsonArrayConst p)
	: Loom_AS7263( EXPAND_ARRAY(p, 6) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::print_config()
{
	LoomI2CSensor::print_config();	
	LPrintln("\t", "Gain                : ", gain);
	LPrintln("\t", "Mode                : ", mode);
	LPrintln("\t", "Use Bulb            : ", (use_bulb) ? "True" : "False");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "NIR R: ", nir_vals[0]);
	LPrintln("\t", "NIR S: ", nir_vals[1]);
	LPrintln("\t", "NIR T: ", nir_vals[2]);
	LPrintln("\t", "NIR U: ", nir_vals[3]);
	LPrintln("\t", "NIR V: ", nir_vals[4]);
	LPrintln("\t", "NIR W: ", nir_vals[5]);
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
	package_json(json, module_name, 
		"NIR_R",	nir_vals[0],
		"NIR_S",	nir_vals[1],
		"NIR_T",	nir_vals[2],
		"NIR_U",	nir_vals[3],
		"NIR_V",	nir_vals[4],
		"NIR_W",	nir_vals[5]
	);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::enable_bulb(bool enable)
{
	use_bulb = enable;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::set_gain(byte gain)
{
	inst_AS7263.setGain(gain);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::set_mode(byte mode)
{
	inst_AS7263.setMeasurementMode(mode);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7263::set_integration_time(byte time)
{
	inst_AS7263.setIntegrationTime(time);
}

///////////////////////////////////////////////////////////////////////////////


