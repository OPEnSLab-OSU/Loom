///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_LogPlat.cpp
/// @brief		File for LoomLogPlat implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "LogPlat.h"
#include "Manager.h"


///////////////////////////////////////////////////////////////////////////////
LoomLogPlat::LoomLogPlat(
		LoomManager* 			manager,
		const char* 						module_name,
		const LoomModule::Type	module_type,
		const bool							enable_rate_filter,
		const uint16_t					min_filter_delay
	)
	: LoomModule(manager, module_name, module_type )
	, enable_rate_filter(enable_rate_filter)
	, min_filter_delay(min_filter_delay)
	, last_log_millis(0)
{}

///////////////////////////////////////////////////////////////////////////////
void LoomLogPlat::print_config() const
{
  LMark;
	LoomModule::print_config();
  LMark;

	LPrintln("\tEnable Log Filter   : ", (enable_rate_filter) ? "Enabled" : "Disabled" );
  LMark;
	if (enable_rate_filter) {
   	LMark;
		LPrintln("\tMinimum Filter Delay: ", min_filter_delay );
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
bool LoomLogPlat::check_millis()
{
  LMark;
	if ( (millis() > min_filter_delay) && ( (millis()-last_log_millis) < min_filter_delay ) ) {
   	LMark;
		print_module_label();
   	LMark;
		LPrintln("Not enough time since last log, need at least ", min_filter_delay, "ms at current settings");
   	LMark;
		return false;
	} else {
		last_log_millis = millis();
   	LMark;
		return true;
	}
}

///////////////////////////////////////////////////////////////////////////////
bool LoomLogPlat::log()
{
  LMark;
	if (device_manager != nullptr) {
   	LMark;
		JsonObject tmp = device_manager->internal_json();
   	LMark;
		if (strcmp(tmp["type"], "data") == 0 ) {
    	LMark;
			return log(tmp);
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
