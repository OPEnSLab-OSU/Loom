///////////////////////////////////////////////////////////////////////////////
///
/// @file		ActuatorTemplate.cpp
/// @brief		File for ActuatorTemplate implementation
/// @author		FirstName LastName
/// @date		Year
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_ACTUATORS

#include "[Template]_Actuator.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
ActuatorTemplate::ActuatorTemplate(
		int						ex_param
	)
	: Actuator( "ActuatorTempate" )
{
	this->module_type = Module::Type::ActuatorTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
ActuatorTemplate::ActuatorTemplate(JsonArrayConst p)
	: ActuatorTemplate( EXPAND_ARRAY(p, 1) ) {}

///////////////////////////////////////////////////////////////////////////////
void ActuatorTemplate::print_config()
{
	Actuator::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void ActuatorTemplate::print_state()
{
	Actuator::print_state();
	// print information about ActuatorTemplate state
}

///////////////////////////////////////////////////////////////////////////////
void ActuatorTemplate::package(JsonObject json)
{
	package_json(json, module_name,
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool ActuatorTemplate::dispatch(JsonObject json)
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

#endif // ifdef LOOM_INCLUDE_ACTUATORS
