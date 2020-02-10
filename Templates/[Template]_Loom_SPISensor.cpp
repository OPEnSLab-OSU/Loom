
#include "[Template]_Loom_SPISensor.h"


///////////////////////////////////////////////////////////////////////////////
LoomSPISensorTemplate::LoomSPISensorTemplate(
		byte			i2c_address, 
		int				num_samples
	) 
	: LoomSPISensor( module_name, num_samples )
{
	this->module_type = LoomModule::Type::SPISensorTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
LoomSPISensorTemplate::LoomSPISensorTemplate(LoomManager* manager, JsonArrayConst p)
	: LoomSPISensorTemplate( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void LoomSPISensorTemplate::print_config()
{
	LoomSPISensor::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void LoomSPISensorTemplate::print_state()
{
	LoomSPISensor::print_state();
	// print information about LoomSPISensorTemplate state 
}

///////////////////////////////////////////////////////////////////////////////
void LoomSPISensorTemplate::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Key1  : ", val1);
	LPrintln("\t", "Key2  : ", val2);
}

///////////////////////////////////////////////////////////////////////////////
void LoomSPISensorTemplate::measure()
{
	// measure data from sensor
}

///////////////////////////////////////////////////////////////////////////////
void LoomSPISensorTemplate::package(JsonObject json)
{
	package_json(json, module_name, 
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool LoomSPISensorTemplate::dispatch(JsonObject json)
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
void LoomSPISensorTemplate::calibrate()
{

}

///////////////////////////////////////////////////////////////////////////////
	