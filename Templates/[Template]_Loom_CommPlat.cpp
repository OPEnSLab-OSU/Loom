///////////////////////////////////////////////////////////////////////////////
///
/// @file		CommPlatTemplate.cpp
/// @brief		File for CommPlatTemplate implementation.
/// @author		FirstName LastName
/// @date		Year
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_RADIOS

#include "[Template]_Loom_CommPlat.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
CommPlatTemplate::CommPlatTemplate(
		uint			max_message_len
	)
	: CommPlat( "CommPlatTemplate", max_message_len )
{
	this->module_type = Module::Type::CommPlatTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
CommPlatTemplate::CommPlatTemplate(JsonArrayConst p)
	: CommPlatTemplate( EXPAND_ARRAY(p, 1) ) {}

///////////////////////////////////////////////////////////////////////////////
void CommPlatTemplate::print_config()
{
	CommPlat::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void CommPlatTemplate::print_state()
{
	CommPlat::print_state();
	// print information about CommPlatTemplate state
}

///////////////////////////////////////////////////////////////////////////////
void CommPlatTemplate::package(JsonObject json)
{
	package_json(json, module_name,
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool CommPlatTemplate::dispatch(JsonObject json)
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
void CommPlatTemplate::set_address(uint addr)
{

}

///////////////////////////////////////////////////////////////////////////////
uint CommPlatTemplate::get_address()
{

}
///////////////////////////////////////////////////////////////////////////////
bool CommPlatTemplate::receive(JsonObject json)
{

}
///////////////////////////////////////////////////////////////////////////////
bool CommPlatTemplate::send(JsonObject json, uint16_t destination)
{

}
///////////////////////////////////////////////////////////////////////////////
bool CommPlatTemplate::send(JsonObject json)
{

}
///////////////////////////////////////////////////////////////////////////////
void CommPlatTemplate::broadcast(JsonObject json)
{

}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_RADIOS
