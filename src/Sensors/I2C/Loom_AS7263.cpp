
#include "Loom_AS7263.h"


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
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
	this->module_type = ModuleType::AS7263;

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

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_AS7263::Loom_AS7263(JsonVariant p)
	: Loom_AS7263(p[0], p[1], p[2], p[3], p[4], p[5])
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_AS7263::~Loom_AS7263() 
{

}

/////////////////////////////////////////////////////////////////////
void Loom_AS7263::print_config()
{
	LoomI2CSensor::print_config();	
	LPrintln("\t", "Gain                : ", gain);
	LPrintln("\t", "Mode                : ", mode);
	LPrintln("\t", "Use Bulb            : ", (use_bulb) ? "True" : "False");
}

/////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////
void Loom_AS7263::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	append_to_bundle(bndl, id_prefix, "NIR_R", nir_vals[0], NEW_MSG);
	append_to_bundle(bndl, id_prefix, "NIR_S", nir_vals[1]);
	append_to_bundle(bndl, id_prefix, "NIR_T", nir_vals[2]);
	append_to_bundle(bndl, id_prefix, "NIR_U", nir_vals[3]);
	append_to_bundle(bndl, id_prefix, "NIR_V", nir_vals[4]);
	append_to_bundle(bndl, id_prefix, "NIR_W", nir_vals[5]);
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7263::enable_bulb(bool enable)
{
	use_bulb = enable;
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7263::set_gain(byte gain)
{
	inst_AS7263.setGain(gain);
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7263::set_mode(byte mode)
{
	inst_AS7263.setMeasurementMode(mode);
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7263::set_integration_time(byte time)
{
	inst_AS7263.setIntegrationTime(time);
}
