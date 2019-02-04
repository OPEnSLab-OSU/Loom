
#include "Loom_AS7262.h"




// --- CONSTRUCTOR ---
Loom_AS7262::Loom_AS7262(byte i2c_address, char* module_name, char* sensor_description, bool use_bulb, int gain, int mode)

	: LoomI2CSensor( module_name, sensor_description, i2c_address )
{
	this->gain     = gain;
	this->mode     = mode;
	this->use_bulb = use_bulb;

	inst_AS7262.begin(Wire, gain, mode);
	// bool setup = inst_AS7262.begin(Wire, gain, mode);

	print_module_label();
	LOOM_DEBUG_Println2("\t", "Initialized");
	// LOOM_DEBUG_Println3("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}


// --- DESTRUCTOR ---
Loom_AS7262::~Loom_AS7262() 
{

}


void Loom_AS7262::print_config()
{
	LoomI2CSensor::print_config();	
	LOOM_DEBUG_Println3("\t", "Gain                : ", gain);
	LOOM_DEBUG_Println3("\t", "Mode                : ", mode);
	LOOM_DEBUG_Println3("\t", "Use Bulb            : ", (use_bulb) ? "True" : "False");
}


void Loom_AS7262::print_measurements()
{
	print_module_label();
	LOOM_DEBUG_Println("Measurements:");
	LOOM_DEBUG_Println3("\t", "Violet : ", violet);
	LOOM_DEBUG_Println3("\t", "Blue   : ", blue);
	LOOM_DEBUG_Println3("\t", "Green  : ", green);
	LOOM_DEBUG_Println3("\t", "Yellow : ", yellow);
	LOOM_DEBUG_Println3("\t", "Orange : ", orange);
	LOOM_DEBUG_Println3("\t", "Red    : ", red);
}


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


void Loom_AS7262::package(OSCBundle* bndl)
{
	char id_prefix[30]; 
	resolve_package_prefix(id_prefix);

	append_to_bundle_key_value(bndl, id_prefix, "Violet", violet);
	append_to_bundle_key_value(bndl, id_prefix, "Blue"  , blue);
	append_to_bundle_key_value(bndl, id_prefix, "Green" , green);
	append_to_bundle_key_value(bndl, id_prefix, "Yellow", yellow);
	append_to_bundle_key_value(bndl, id_prefix, "Orange", orange);
	append_to_bundle_key_value(bndl, id_prefix, "Red"   , red);
}


void Loom_AS7262::package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port)
{
	LoomI2CSensor::package_mux(bndl, id_prefix, port);

	append_to_bundle_key_value(bndl, id_prefix, "Violet", violet);
	append_to_bundle_key_value(bndl, id_prefix, "Blue"  , blue);
	append_to_bundle_key_value(bndl, id_prefix, "Green" , green);
	append_to_bundle_key_value(bndl, id_prefix, "Yellow", yellow);
	append_to_bundle_key_value(bndl, id_prefix, "Orange", orange);
	append_to_bundle_key_value(bndl, id_prefix, "Red"   , red);
}


void Loom_AS7262::enable_bulb(bool e)
{
	use_bulb = e;
}

