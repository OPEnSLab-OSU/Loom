
#include "Loom_LogPlat.h"


/////////////////////////////////////////////////////////////////////
char* LoomLogPlat::enum_log_plat_string(LogPlatform p)
{
	switch(p) {
		case LogPlatform::SDCARD     : return "SD Card";
		case LogPlatform::PUSHINGBOX : return "PushingBox";
		case LogPlatform::OLED       : return "OLED";
		case LogPlatform::SERIAL_MON : return "Serial";
	}
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
LoomLogPlat::LoomLogPlat( 
		const char*		module_name, 
		bool			enable_rate_filter,
		uint			min_filter_delay
	)
	: LoomModule( module_name )
{
	this->enable_rate_filter	= enable_rate_filter;
	this->min_filter_delay 		= min_filter_delay;
	this->last_log_millis		= 0;  					// Not sure if there will be any issue with the first log 
}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
LoomLogPlat::~LoomLogPlat() {}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void LoomLogPlat::print_config() 
{
	LoomModule::print_config();

	LPrintln('\t', "Enable Log Filter   : ", (enable_rate_filter) ? "Enabled" : "Disabled" );
	if (enable_rate_filter) {
		LPrintln('\t', "Minimum Filter Delay: ", min_filter_delay );
	}
}

/////////////////////////////////////////////////////////////////////
void LoomLogPlat::measure() 
{

}

/////////////////////////////////////////////////////////////////////
bool LoomLogPlat::check_millis() 
{
	if ( (millis() > min_filter_delay) && ( (millis()-last_log_millis) < min_filter_delay ) ) {
		// LPrintln("FALSE");
		// LPrintln("millis: ", millis() );
		// LPrintln("last millis: ", last_log_millis );
		// LPrintln("delay: ", min_filter_delay );
		return false;
	} else {
		last_log_millis = millis();
		return true;
	}
}

