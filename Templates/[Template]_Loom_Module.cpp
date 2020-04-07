
#include "[Template]_Loom_Module.h"


///////////////////////////////////////////////////////////////////////////////
LoomModuleTemplate::LoomModuleTemplate(
		LoomManager* manager,
		const char* module_name, 
		int				ex_param
	) 
	: LoomModule(manager, module_name )
{
	this->module_type = LoomModule::Type::ModuleTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
LoomModuleTemplate::LoomModuleTemplate(LoomManager* manager, JsonArrayConst p)
	: LoomModuleTemplate( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void LoomModuleTemplate::print_config()
{
	LoomModule::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void LoomModuleTemplate::print_state()
{
	LoomModule::print_state();
	// print information about LoomModuleTemplate state 
}

///////////////////////////////////////////////////////////////////////////////
void LoomModuleTemplate::package(JsonObject json)
{
	package_json(json, module_name, 
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool LoomModuleTemplate::dispatch(JsonObject json)
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


