///////////////////////////////////////////////////////////////////////////////
///
/// @file		LogPlatTemplate.h
/// @brief		File for LogPlatTemplate implementation
/// @author		FirstName LastName
/// @date		Year
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "[Template]_Loom_LogPlat.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
LogPlatTemplate::LogPlatTemplate(
		bool			enable_rate_filter,
		uint			min_filter_delay,
	)
	: LogPlat( "LogPlatTemplate", enable_rate_filter, min_filter_delay )
{
	this->module_type = Module::Type::LogPlatTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
LogPlatTemplate::LogPlatTemplate(JsonArrayConst p)
	: LogPlatTemplate( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void LogPlatTemplate::print_config()
{
	LogPlat::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void LogPlatTemplate::print_state()
{
	LogPlat::print_state();
	// print information about LogPlatTemplate state
}

///////////////////////////////////////////////////////////////////////////////
void LogPlatTemplate::package(JsonObject json)
{
	package_json(json, module_name,
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool LogPlatTemplate::dispatch(JsonObject json)
{
	if ( strcmp(json["module"], module_name) == 0 ) {
		JsonArray params = json["params"];
		return functionRoute(
			json["func"],
			"method_to_run", [this, params]() { if (params.size() >= 5) { method_to_run( EXPAND_ARRAY(params, 5) ); } else { LPrintln("Not enough parameters"); } }
		);
		// The `5`s in the above command correspond to the number of parameters to `method_to_run`
		// Change to match the parameters of your method
	} else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////
void LogPlatTemplate::log(JsonObject json)
{

}

///////////////////////////////////////////////////////////////////////////////
