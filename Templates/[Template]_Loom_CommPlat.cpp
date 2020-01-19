
#include "[Template]_Loom_CommPlat.h"


///////////////////////////////////////////////////////////////////////////////
LoomCommPlatTemplate::LoomCommPlatTemplate(
LoomManager* manager,
const char* module_name, 
		uint			max_message_len
	) 
	: LoomCommPlat( module_name, max_message_len )
{
	this->module_type = LoomModule::Type::CommPlatTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
LoomCommPlatTemplate::LoomCommPlatTemplate(LoomManager* manager, JsonArrayConst p)
	: LoomCommPlatTemplate( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void LoomCommPlatTemplate::print_config()
{
	LoomCommPlat::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void LoomCommPlatTemplate::print_state()
{
	LoomCommPlat::print_state();
	// print information about LoomCommPlatTemplate state 
}

///////////////////////////////////////////////////////////////////////////////
void LoomCommPlatTemplate::package(JsonObject json)
{
	package_json(json, module_name, 
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool LoomCommPlatTemplate::dispatch(JsonObject json)
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
void LoomCommPlatTemplate::set_address(uint addr)
{ 

}

///////////////////////////////////////////////////////////////////////////////
uint LoomCommPlatTemplate::get_address() 
{ 

}
///////////////////////////////////////////////////////////////////////////////
bool LoomCommPlatTemplate::receive(JsonObject json)
{

}
///////////////////////////////////////////////////////////////////////////////
bool LoomCommPlatTemplate::send(JsonObject json, uint16_t destination)
{

}
///////////////////////////////////////////////////////////////////////////////
bool LoomCommPlatTemplate::send(JsonObject json)
{

}
///////////////////////////////////////////////////////////////////////////////
void LoomCommPlatTemplate::broadcast(JsonObject json) 
{

}

