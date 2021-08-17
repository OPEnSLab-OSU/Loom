///////////////////////////////////////////////////////////////////////////////
///
/// @file		InternetPlatTemplate.cpp
/// @brief		File for InternetPlatTemplate implementation
/// @author		FirstName LastName
/// @date		Year
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_ETHERNET

#include "[Template]_Loom_InternetPlat.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
InternetPlatTemplate::InternetPlatTemplate(
		int				ex_param
	)
	: InternetPlat( "InternetPlatTemplate" )
{
	this->module_type = Module::Type::InternetPlatTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
InternetPlatTemplate::InternetPlatTemplate(JsonArrayConst p)
	: InternetPlatTemplate( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void InternetPlatTemplate::print_config()
{
	InternetPlat::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void InternetPlatTemplate::print_state()
{
	InternetPlat::print_state();
	// print information about InternetPlatTemplate state
}

///////////////////////////////////////////////////////////////////////////////
void InternetPlatTemplate::package(JsonObject json)
{
	package_json(json, module_name,
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool InternetPlatTemplate::dispatch(JsonObject json)
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
void InternetPlatTemplate::connect()
{

}

///////////////////////////////////////////////////////////////////////////////
bool InternetPlatTemplate::is_connected()
{

}

///////////////////////////////////////////////////////////////////////////////
uint32_t InternetPlatTemplate::get_time()
{

}

///////////////////////////////////////////////////////////////////////////////
Client& InternetPlatTemplate::http_request(const char* domain, const char* url, const char* body, const char* verb)
{

}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_ETHERNET
