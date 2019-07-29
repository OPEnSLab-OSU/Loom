
#include "Loom_LogPlat.h"
#include "Loom_Manager.h"


///////////////////////////////////////////////////////////////////////////////
const char* LoomLogPlat::enum_log_plat_string(Platform p)
{
	switch(p) {
		case Platform::SDCARD     : return "SD Card";
		case Platform::PUSHINGBOX : return "PushingBox";
		case Platform::OLED       : return "OLED";
		case Platform::SERIAL_MON : return "Serial";
	}
}

///////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
void LoomLogPlat::print_config() 
{
	LoomModule::print_config();

	LPrintln('\t', "Enable Log Filter   : ", (enable_rate_filter) ? "Enabled" : "Disabled" );
	if (enable_rate_filter) {
		LPrintln('\t', "Minimum Filter Delay: ", min_filter_delay );
	}
}

///////////////////////////////////////////////////////////////////////////////
bool LoomLogPlat::check_millis() 
{
	if ( (millis() > min_filter_delay) && ( (millis()-last_log_millis) < min_filter_delay ) ) {
		print_module_label();
		LPrintln("Not enough time since last log, need at least ", min_filter_delay, "ms at current settings");
		return false;
	} else {
		last_log_millis = millis();
		return true;
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomLogPlat::log()
{
	if (device_manager != nullptr) {
		JsonObject tmp = device_manager->internal_json();
		if (strcmp(tmp["type"], "data") == 0 ) {
			log(tmp);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
