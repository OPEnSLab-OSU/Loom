
#include "[Template]_Loom_I2CSensor.h"


///////////////////////////////////////////////////////////////////////////////
LoomI2CSensorTemplate::LoomI2CSensorTemplate(
		byte			i2c_address, 
		LoomManager* manager,
const char* module_name 
	) 
	: LoomI2CSensor( module_name, i2c_address )
{
	this->module_type = LoomModule::Type::I2CSensorTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
LoomI2CSensorTemplate::LoomI2CSensorTemplate(LoomManager* manager, JsonArrayConst p)
	: LoomI2CSensorTemplate( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void LoomI2CSensorTemplate::print_config()
{
	LoomI2CSensor::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void LoomI2CSensorTemplate::print_state()
{
	LoomI2CSensor::print_state();
	// print information about LoomI2CSensorTemplate state 
}

///////////////////////////////////////////////////////////////////////////////
void LoomI2CSensorTemplate::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Key1  : ", val1);
	LPrintln("\t", "Key2  : ", val2);
}

///////////////////////////////////////////////////////////////////////////////
void LoomI2CSensorTemplate::measure()
{
	// measure data from sensor
}

///////////////////////////////////////////////////////////////////////////////
void LoomI2CSensorTemplate::package(JsonObject json)
{
	package_json(json, module_name, 
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool LoomI2CSensorTemplate::dispatch(JsonObject json)
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
void LoomI2CSensorTemplate::calibrate()
{

}

///////////////////////////////////////////////////////////////////////////////
	