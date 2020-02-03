
#include "[Template]_Loom_PublishPlat.h"


///////////////////////////////////////////////////////////////////////////////
LoomPublishPlatTemplate::LoomPublishPlatTemplate(
		LoomManager* manager,
const char* module_name, 
		uint			internet_index, 
		int				ex_param, 
	) 
	: LoomPublishPlat( module_name, internet_index )
{
	this->module_type = LoomModule::Type::PublishPlatTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
LoomPublishPlatTemplate::LoomPublishPlatTemplate(LoomManager* manager, JsonArrayConst p)
	: LoomPublishPlatTemplate( EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void LoomPublishPlatTemplate::print_config()
{
	LoomPublishPlat::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void LoomPublishPlatTemplate::print_state()
{
	LoomPublishPlat::print_state();
	// print information about LoomPublishPlatTemplate state 
}

///////////////////////////////////////////////////////////////////////////////
void LoomPublishPlatTemplate::package(JsonObject json)
{
	package_json(json, module_name, 
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool LoomPublishPlatTemplate::dispatch(JsonObject json)
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
bool LoomPublishPlatTemplate::send_to_internet(const JsonObject json, LoomInternetPlat* plat)
{
	
}

///////////////////////////////////////////////////////////////////////////////
