
#include "Loom_AS7262.h"


///////////////////////////////////////////////////////////////////////////////
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
	this->module_type = LoomModule::Type::AS7262;

	this->use_bulb 			= use_bulb;
	this->gain 				= gain;
	this->mode 				= mode;
	this->integration_time 	= integration_time;

	inst_AS7262.begin(Wire, gain, mode);
	inst_AS7262.setIntegrationTime(integration_time);

	print_module_label();
	LPrintln("\t", "Initialized");
	// LPrintln("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_AS7262::Loom_AS7262(JsonArrayConst p)
	: Loom_AS7262( EXPAND_ARRAY(p, 6) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7262::print_config()
{
	LoomI2CSensor::print_config();	
	LPrintln("\t", "Gain                : ", gain);
	LPrintln("\t", "Mode                : ", mode);
	LPrintln("\t", "Use Bulb            : ", (use_bulb) ? "True" : "False");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7262::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Violet : ", color_vals[0]);
	LPrintln("\t", "Blue   : ", color_vals[1]);
	LPrintln("\t", "Green  : ", color_vals[2]);
	LPrintln("\t", "Yellow : ", color_vals[3]);
	LPrintln("\t", "Orange : ", color_vals[4]);
	LPrintln("\t", "Red    : ", color_vals[5]);
}

///////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7262::package(JsonObject json)
{
	package_json(json, module_name, 
		"Violet",	color_vals[0],
		"Blue",		color_vals[1],
		"Green",	color_vals[2],
		"Yellow",	color_vals[3],
		"Orange",	color_vals[4],
		"Red",		color_vals[5]
	);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7262::enable_bulb(bool enable)
{
	use_bulb = enable;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7262::set_gain(byte gain)
{
	inst_AS7262.setGain(gain);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7262::set_mode(byte mode)
{
	inst_AS7262.setMeasurementMode(mode);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_AS7262::set_integration_time(byte time)
{
	inst_AS7262.setIntegrationTime(time);
}

///////////////////////////////////////////////////////////////////////////////



