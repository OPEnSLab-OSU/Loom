
#include "Loom_AS7263.h"


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_AS7263::Loom_AS7263(byte i2c_address, char* module_name, char* sensor_description, bool use_bulb, byte gain, byte mode, byte integration_time)

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
	Println3("\t", "NIR R: ", nir_r);
	Println3("\t", "NIR S: ", nir_s);
	Println3("\t", "NIR T: ", nir_t);
	Println3("\t", "NIR U: ", nir_u);
	Println3("\t", "NIR V: ", nir_v);
	Println3("\t", "NIR W: ", nir_w);
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7263::measure()
{
	if (use_bulb) {
		inst_AS7263.takeMeasurementsWithBulb();
	} else {
		inst_AS7263.takeMeasurements();
	}

	nir_r = inst_AS7263.getR();
	nir_s = inst_AS7263.getS();
	nir_t = inst_AS7263.getT();
	nir_u = inst_AS7263.getU();
	nir_v = inst_AS7263.getV();
	nir_w = inst_AS7263.getW();	
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7263::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	append_to_bundle(bndl, id_prefix, "NIR_R", nir_r, NEW_MSG);
	append_to_bundle(bndl, id_prefix, "NIR_S", nir_s);
	append_to_bundle(bndl, id_prefix, "NIR_T", nir_t);
	append_to_bundle(bndl, id_prefix, "NIR_U", nir_u);
	append_to_bundle(bndl, id_prefix, "NIR_V", nir_v);
	append_to_bundle(bndl, id_prefix, "NIR_W", nir_w);
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7263::enable_bulb(bool enable)
{
	use_bulb = enable;
}

/////////////////////////////////////////////////////////////////////
// 0: 1x (power-on default), 1: 3.7x, 2: 16x, 3: 64x
void Loom_AS7263::set_gain(byte gain)
{
	inst_AS7263.setGain(gain);
}

/////////////////////////////////////////////////////////////////////
// 0: Continuous reading of STUV 
// 1: Continuous reading of RTUX 
// 2: Continuous reading of all channels (power-on default)
// 3: One-shot reading of all channels
void Loom_AS7263::set_mode(byte mode)
{
	inst_AS7263.setMeasurementMode(mode);
}

/////////////////////////////////////////////////////////////////////
// Time will be 2.8ms * [integration value]  (0-255), 50 is default
void Loom_AS7263::set_integration_time(byte time)
{
	inst_AS7263.setIntegrationTime(time);
}
