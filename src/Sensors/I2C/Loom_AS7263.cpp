
#include "Loom_AS7263.h"


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_AS7263::Loom_AS7263(
		byte		i2c_address, 
		char*		module_name,
		char*		sensor_description, 
		bool		use_bulb, 
		byte		gain, 
		byte		mode, 
		byte		integration_time
	)
	: LoomI2CSensor( module_name, sensor_description, i2c_address )
{
	this->use_bulb 			= use_bulb;
	this->gain 				= gain;
	this->mode 				= mode;
	this->integration_time 	= integration_time;

	inst_AS7263.begin(Wire, gain, mode);
	inst_AS7263.setIntegrationTime(integration_time);

	print_module_label();
	Println2("\t", "Initialized");
	// Println3("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_AS7263::~Loom_AS7263() 
{

}

/////////////////////////////////////////////////////////////////////
void Loom_AS7263::print_config()
{
	LoomI2CSensor::print_config();	
	Println3("\t", "Gain                : ", gain);
	Println3("\t", "Mode                : ", mode);
	Println3("\t", "Use Bulb            : ", (use_bulb) ? "True" : "False");
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7263::print_measurements()
{
	print_module_label();
	Println("Measurements:");
	Println3("\t", "NIR R: ", nir_vals[0]);
	Println3("\t", "NIR S: ", nir_vals[1]);
	Println3("\t", "NIR T: ", nir_vals[2]);
	Println3("\t", "NIR U: ", nir_vals[3]);
	Println3("\t", "NIR V: ", nir_vals[4]);
	Println3("\t", "NIR W: ", nir_vals[5]);
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
