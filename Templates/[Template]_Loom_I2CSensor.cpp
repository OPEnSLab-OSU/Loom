///////////////////////////////////////////////////////////////////////////////
///
/// @file		I2CSensorTemplate.cpp
/// @brief		File for I2CSensorTemplate implementation.
/// @author		FirstName LastName
/// @date		Year
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "[Template]_Loom_I2CSensor.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
I2CSensorTemplate::I2CSensorTemplate(
		byte			i2c_address,
		uint8_t	mux_port
	)
	: I2CSensor( "I2CSensorTemplate", i2c_address, mux_port )
{
	this->module_type = Module::Type::I2CSensorTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
I2CSensorTemplate::I2CSensorTemplate(JsonArrayConst p)
	: I2CSensorTemplate( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void I2CSensorTemplate::print_config()
{
	I2CSensor::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void I2CSensorTemplate::print_state()
{
	I2CSensor::print_state();
	// print information about I2CSensorTemplate state
}

///////////////////////////////////////////////////////////////////////////////
void I2CSensorTemplate::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Key1  : ", val1);
	LPrintln("\t", "Key2  : ", val2);
}

///////////////////////////////////////////////////////////////////////////////
void I2CSensorTemplate::measure()
{
	// measure data from sensor
}

///////////////////////////////////////////////////////////////////////////////
void I2CSensorTemplate::package(JsonObject json)
{
	package_json(json, module_name,
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool I2CSensorTemplate::dispatch(JsonObject json)
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
void I2CSensorTemplate::calibrate()
{

}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
