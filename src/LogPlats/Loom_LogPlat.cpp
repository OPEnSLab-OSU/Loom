
#include "Loom_LogPlat.h"
#include "Loom_Manager.h"


///////////////////////////////////////////////////////////////////////////////
LoomLogPlat::LoomLogPlat( 
		const char*			module_name, 
		LoomModule::Type	module_type,
		bool				enable_rate_filter,
		uint16_t			min_filter_delay
	)
	: LoomModule( module_name, module_type )
	, enable_rate_filter(enable_rate_filter)
	, min_filter_delay(min_filter_delay)
	, last_log_millis(0)
{}

///////////////////////////////////////////////////////////////////////////////
void LoomLogPlat::print_config() 
{
	LoomModule::print_config();

	LPrintln("\tEnable Log Filter   : ", (enable_rate_filter) ? "Enabled" : "Disabled" );
	if (enable_rate_filter) {
		LPrintln("\tMinimum Filter Delay: ", min_filter_delay );
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
