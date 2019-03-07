
#include "Loom_AS7262.h"



/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_AS7262::Loom_AS7262(byte i2c_address, char* module_name, char* sensor_description, bool use_bulb, byte gain, byte mode, byte integration_time)

	: LoomI2CSensor( module_name, sensor_description, i2c_address )
{
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
	Println3("\t", "Violet : ", violet);
	Println3("\t", "Blue   : ", blue);
	Println3("\t", "Green  : ", green);
	Println3("\t", "Yellow : ", yellow);
	Println3("\t", "Orange : ", orange);
	Println3("\t", "Red    : ", red);
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7262::measure()
{
	if (use_bulb) {
		inst_AS7262.takeMeasurementsWithBulb();
	} else {
		inst_AS7262.takeMeasurements();
	}

	violet = inst_AS7262.getViolet();
	blue   = inst_AS7262.getBlue();
	green  = inst_AS7262.getGreen();
	yellow = inst_AS7262.getYellow();
	orange = inst_AS7262.getOrange();
	red    = inst_AS7262.getRed();	
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7262::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	append_to_bundle(bndl, id_prefix, "Violet", violet, NEW_MSG);
	append_to_bundle(bndl, id_prefix, "Blue"  , blue);
	append_to_bundle(bndl, id_prefix, "Green" , green);
	append_to_bundle(bndl, id_prefix, "Yellow", yellow);
	append_to_bundle(bndl, id_prefix, "Orange", orange);
	append_to_bundle(bndl, id_prefix, "Red"   , red);
}

/////////////////////////////////////////////////////////////////////
void Loom_AS7262::enable_bulb(bool enable)
{
	use_bulb = enable;
}

/////////////////////////////////////////////////////////////////////
// 0: 1x (power-on default), 1: 3.7x, 2: 16x, 3: 64x
void Loom_AS7262::set_gain(byte gain)
{
	inst_AS7262.setGain(gain);
}

/////////////////////////////////////////////////////////////////////
// 0: Continuous reading of VBGY 
// 1: Continuous reading of GYOR 
// 2: Continuous reading of all channels (power-on default)
// 3: One-shot reading of all channels
void Loom_AS7262::set_mode(byte mode)
{
	inst_AS7262.setMeasurementMode(mode);
}

/////////////////////////////////////////////////////////////////////
// Time will be 2.8ms * [integration value]  (0-255), 50 is default
void Loom_AS7262::set_integration_time(byte time)
{
	inst_AS7262.setIntegrationTime(time);
}


