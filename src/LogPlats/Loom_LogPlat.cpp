
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

	Println3('\t', "Enable Log Filter   : ", (enable_rate_filter) ? "Enabled" : "Disabled" );
	if (enable_rate_filter) {
		Println3('\t', "Minimum Filter Delay: ", min_filter_delay );
	}
}

/////////////////////////////////////////////////////////////////////
void LoomLogPlat::package(OSCBundle& bndl, char* suffix) 
{

}

/////////////////////////////////////////////////////////////////////
void LoomLogPlat::measure() 
{

}

/////////////////////////////////////////////////////////////////////
bool LoomLogPlat::message_route(OSCMessage& msg, int address_offset) 
{
	// maybe set subnet_scope 
}

/////////////////////////////////////////////////////////////////////
bool LoomLogPlat::check_millis() 
{
	if ( (millis() > min_filter_delay) && ( (millis()-last_log_millis) < min_filter_delay ) ) {
		// Println("FALSE");
		// Println2("millis: ", millis() );
		// Println2("last millis: ", last_log_millis );
		// Println2("delay: ", min_filter_delay );
		return false;
	} else {
		last_log_millis = millis();
		return true;
	}
}

