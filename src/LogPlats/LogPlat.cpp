///////////////////////////////////////////////////////////////////////////////
///
/// @file		LogPlat.cpp
/// @brief		File for LogPlat implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "LogPlat.h"
#include "Manager.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
LogPlat::LogPlat( 
		const char*				module_name, 
		const Module::Type	module_type,
		const bool				enable_rate_filter,
		const uint16_t			min_filter_delay
	)
	: Module(module_name, module_type)
	, enable_rate_filter(enable_rate_filter)
	, min_filter_delay(min_filter_delay)
	, last_log_millis(0)
{}

///////////////////////////////////////////////////////////////////////////////
void LogPlat::print_config() const
{
	Module::print_config();

	LPrintln("\tEnable Log Filter   : ", (enable_rate_filter) ? "Enabled" : "Disabled" );
	if (enable_rate_filter) {
		LPrintln("\tMinimum Filter Delay: ", min_filter_delay );
	}
}

///////////////////////////////////////////////////////////////////////////////
bool LogPlat::check_millis() 
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
bool LogPlat::log()
{
	if (device_manager != nullptr) {
		JsonObject tmp = device_manager->internal_json();
		if (strcmp(tmp["type"], "data") == 0 ) {
			return log(tmp);
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
