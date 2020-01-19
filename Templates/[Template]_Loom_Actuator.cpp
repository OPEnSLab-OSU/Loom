
#include "[Template]_Loom_Actuator.h"


///////////////////////////////////////////////////////////////////////////////
LoomActuatorTemplate::LoomActuatorTemplate(
		LoomManager*	manager,
		const char*		module_name, 
		int						ex_param
	) 
	: LoomActuator( module_name )
{
	this->module_type = LoomModule::Type::ActuatorTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
LoomActuatorTemplate::LoomActuatorTemplate(LoomManager* manager, JsonArrayConst p)
	: LoomActuatorTemplate( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void LoomActuatorTemplate::print_config()
{
	LoomActuator::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void LoomActuatorTemplate::print_state()
{
	LoomActuator::print_state();
	// print information about LoomActuatorTemplate state 
}

///////////////////////////////////////////////////////////////////////////////
void LoomActuatorTemplate::package(JsonObject json)
{
	package_json(json, module_name, 
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool LoomActuatorTemplate::dispatch(JsonObject json)
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


