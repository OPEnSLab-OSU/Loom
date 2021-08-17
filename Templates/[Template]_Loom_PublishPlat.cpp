///////////////////////////////////////////////////////////////////////////////
///
/// @file		PublishPlatTemplate.h
/// @brief		File for PublishPlatTemplate implementation
/// @author		FirstName LastName
/// @date		Year
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))

#include "[Template]_Loom_PublishPlat.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
PublishPlatTemplate::PublishPlatTemplate(
		int				ex_param,
	)
	: PublishPlat( "PublishPlatTemplate" )
{
	this->module_type = Module::Type::PublishPlatTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
PublishPlatTemplate::PublishPlatTemplate(JsonArrayConst p)
	: PublishPlatTemplate( EXPAND_ARRAY(p, 1) ) {}

///////////////////////////////////////////////////////////////////////////////
void PublishPlatTemplate::print_config()
{
	PublishPlat::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void PublishPlatTemplate::print_state()
{
	PublishPlat::print_state();
	// print information about PublishPlatTemplate state
}

///////////////////////////////////////////////////////////////////////////////
void PublishPlatTemplate::package(JsonObject json)
{
	package_json(json, module_name,
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool PublishPlatTemplate::dispatch(JsonObject json)
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
bool PublishPlatTemplate::send_to_internet(const JsonObject json, InternetPlat* plat)
{

}

///////////////////////////////////////////////////////////////////////////////

#endif // if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
