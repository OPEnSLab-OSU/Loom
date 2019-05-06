
#include "Loom_AS7262.h"



/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_AS7262::Loom_AS7262(
		byte			i2c_address, 
		const char*		module_name, 
		bool			use_bulb, 
		byte			gain, 
		byte			mode, 
		byte			integration_time
	)
	: LoomI2CSensor( module_name, i2c_address )
{
	this->module_type = ModuleType::AS7262;

	this->use_bulb 			= use_bulb;
	this->gain 				= gain;
	this->mode 				= mode;
	this->integration_time 	= integration_time;

	inst_AS7262.begin(Wire, gain, mode);
	inst_AS7262.setIntegrationTime(integration_time);

	print_module_label();
	Println2("\t", "Initialized");
	// Println3("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_AS7262::Loom_AS7262(JsonVariant p)
	: Loom_AS7262(p[0], p[1], p[2], p[3], p[4], p[5])
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_AS7262::~Loom_AS7262() 
{

}

/////////////////////////////////////////////////////////////////////
void Loom_AS7262::print_config()
{
	LoomI2CSensor::print_config();	
	Println3("\t", "Gain                : ", gain);
	Println3("\t", "Mode                : ", mode);
	Println3("\t", "Use Bulb            : ", (use_bulb) ? "True" : "False");
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7262::print_measurements()
{
	print_module_label();
	Println("Measurements:");
	Println3("\t", "Violet : ", color_vals[0]);
	Println3("\t", "Blue   : ", color_vals[1]);
	Println3("\t", "Green  : ", color_vals[2]);
	Println3("\t", "Yellow : ", color_vals[3]);
	Println3("\t", "Orange : ", color_vals[4]);
	Println3("\t", "Red    : ", color_vals[5]);
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7262::measure()
{
	if (use_bulb) {
		inst_AS7262.takeMeasurementsWithBulb();
	} else {
		inst_AS7262.takeMeasurements();
	}

	color_vals[0] = inst_AS7262.getViolet();
	color_vals[1] = inst_AS7262.getBlue();
	color_vals[2] = inst_AS7262.getGreen();
	color_vals[3] = inst_AS7262.getYellow();
	color_vals[4] = inst_AS7262.getOrange();
	color_vals[5] = inst_AS7262.getRed();	
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7262::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	append_to_bundle(bndl, id_prefix, "Violet", color_vals[0], NEW_MSG);
	append_to_bundle(bndl, id_prefix, "Blue"  , color_vals[1]);
	append_to_bundle(bndl, id_prefix, "Green" , color_vals[2]);
	append_to_bundle(bndl, id_prefix, "Yellow", color_vals[3]);
	append_to_bundle(bndl, id_prefix, "Orange", color_vals[4]);
	append_to_bundle(bndl, id_prefix, "Red"   , color_vals[5]);
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7262::enable_bulb(bool enable)
{
	use_bulb = enable;
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7262::set_gain(byte gain)
{
	inst_AS7262.setGain(gain);
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7262::set_mode(byte mode)
{
	inst_AS7262.setMeasurementMode(mode);
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7262::set_integration_time(byte time)
{
	inst_AS7262.setIntegrationTime(time);
}


