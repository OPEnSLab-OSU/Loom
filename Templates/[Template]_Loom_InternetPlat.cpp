
#include "[Template]_Loom_InternetPlat.h"


///////////////////////////////////////////////////////////////////////////////
LoomInternetPlatTemplate::LoomInternetPlatTemplate(
		LoomManager* manager,
const char* module_name, 
		int				ex_param
	) 
	: LoomInternetPlat( module_name )
{
	this->module_type = LoomModule::Type::InternetPlatTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
LoomInternetPlatTemplate::LoomInternetPlatTemplate(LoomManager* manager, JsonArrayConst p)
	: LoomInternetPlatTemplate( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void LoomInternetPlatTemplate::print_config()
{
	LoomInternetPlat::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void LoomInternetPlatTemplate::print_state()
{
	LoomInternetPlat::print_state();
	// print information about LoomInternetPlatTemplate state 
}

///////////////////////////////////////////////////////////////////////////////
void LoomInternetPlatTemplate::package(JsonObject json)
{
	package_json(json, module_name, 
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool LoomInternetPlatTemplate::dispatch(JsonObject json)
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
void LoomInternetPlatTemplate::connect()
{

}

///////////////////////////////////////////////////////////////////////////////
bool LoomInternetPlatTemplate::is_connected()
{

}

///////////////////////////////////////////////////////////////////////////////
uint32_t LoomInternetPlatTemplate::get_time()
{

}

///////////////////////////////////////////////////////////////////////////////
Client& LoomInternetPlatTemplate::http_request(const char* domain, const char* url, const char* body, const char* verb)
{

}

///////////////////////////////////////////////////////////////////////////////





