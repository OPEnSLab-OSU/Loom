
#include "[Template]_Loom_LogPlat.h"


///////////////////////////////////////////////////////////////////////////////
LoomLogPlatTemplate::LoomLogPlatTemplate(
		LoomManager* manager,
const char* module_name, 
		bool			enable_rate_filter, 
		uint			min_filter_delay, 
	) 
	: LoomLogPlat( module_name, enable_rate_filter, min_filter_delay )
{
	this->module_type = LoomModule::Type::LogPlatTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
LoomLogPlatTemplate::LoomLogPlatTemplate(LoomManager* manager, JsonArrayConst p)
	: LoomLogPlatTemplate( EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void LoomLogPlatTemplate::print_config()
{
	LoomLogPlat::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void LoomLogPlatTemplate::print_state()
{
	LoomLogPlat::print_state();
	// print information about LoomLogPlatTemplate state 
}

///////////////////////////////////////////////////////////////////////////////
void LoomLogPlatTemplate::package(JsonObject json)
{
	package_json(json, module_name, 
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool LoomLogPlatTemplate::dispatch(JsonObject json)
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
void LoomLogPlatTemplate::log(JsonObject json) 
{
	
}

///////////////////////////////////////////////////////////////////////////////
