///////////////////////////////////////////////////////////////////////////////
///
/// @file		SDI12SensorTemplate.cpp
/// @brief		File for SDI12SensorTemplate implementation.
/// @author		FirstName LastName
/// @date		Year
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "[Template]_Loom_SDI12_Sensor.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
SDI12SensorTemplate::SDI12SensorTemplate(
const char* module_name,
		int				num_samples
	)
	: SDI12Sensor( module_name, num_samples )
{
	this->module_type = Module::Type::SDI12SensorTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
SDI12SensorTemplate::SDI12SensorTemplate(JsonArrayConst p)
	: SDI12SensorTemplate( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void SDI12SensorTemplate::print_config()
{
	SDI12Sensor::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void SDI12SensorTemplate::print_state()
{
	SDI12Sensor::print_state();
	// print information about SDI12SensorTemplate state
}

///////////////////////////////////////////////////////////////////////////////
void SDI12SensorTemplate::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Key1  : ", val1);
	LPrintln("\t", "Key2  : ", val2);
}

///////////////////////////////////////////////////////////////////////////////
void SDI12SensorTemplate::measure()
{
	// measure data from sensor
}

///////////////////////////////////////////////////////////////////////////////
void SDI12SensorTemplate::package(JsonObject json)
{
	package_json(json, module_name,
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool SDI12SensorTemplate::dispatch(JsonObject json)
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
void SDI12SensorTemplate::calibrate()
{

}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
