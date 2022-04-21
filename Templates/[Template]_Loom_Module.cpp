///////////////////////////////////////////////////////////////////////////////
///
/// @file		ModuleTemplate.h
/// @brief		File for ModuleTemplate implementation
/// @author		FirstName LastName
/// @date		Year
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "[Template]_Loom_Module.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
ModuleTemplate::ModuleTemplate(
		int				ex_param
	)
	: Module( "ModuleTemplate" )
{
	this->module_type = Module::Type::ModuleTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
ModuleTemplate::ModuleTemplate(JsonArrayConst p)
	: ModuleTemplate( EXPAND_ARRAY(p, 1) ) {}

///////////////////////////////////////////////////////////////////////////////
void ModuleTemplate::print_config()
{
	Module::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void ModuleTemplate::print_state()
{
	Module::print_state();
	// print information about ModuleTemplate state
}

///////////////////////////////////////////////////////////////////////////////
void ModuleTemplate::package(JsonObject json)
{
	package_json(json, module_name,
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool ModuleTemplate::dispatch(JsonObject json)
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
