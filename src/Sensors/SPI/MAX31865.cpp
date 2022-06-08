///////////////////////////////////////////////////////////////////////////////
///
/// @file		MAX31865.cpp
/// @brief		File for MAX31865 implementation.
/// @author		Kurtis Dinelle
/// @date		2022
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#define LOOM_INCLUDE_SENSORS // DELETE ME
#ifdef LOOM_INCLUDE_SENSORS

#include "MAX31865.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
SPISensorTemplate::SPISensorTemplate(
		char* 		module_name,
		int				num_samples
	)
	: SPISensor( module_name, num_samples )
{
	this->module_type = Module::Type::SPISensorTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
SPISensorTemplate::SPISensorTemplate(JsonArrayConst p)
	: SPISensorTemplate( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void SPISensorTemplate::print_config()
{
	SPISensor::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void SPISensorTemplate::print_state()
{
	SPISensor::print_state();
	// print information about SPISensorTemplate state
}

///////////////////////////////////////////////////////////////////////////////
void SPISensorTemplate::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Key1  : ", val1);
	LPrintln("\t", "Key2  : ", val2);
}

///////////////////////////////////////////////////////////////////////////////
void SPISensorTemplate::measure()
{
	// measure data from sensor
}

///////////////////////////////////////////////////////////////////////////////
void SPISensorTemplate::package(JsonObject json)
{
	package_json(json, module_name,
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool SPISensorTemplate::dispatch(JsonObject json)
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
void SPISensorTemplate::calibrate()
{

}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
