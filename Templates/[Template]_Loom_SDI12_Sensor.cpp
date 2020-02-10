
#include "[Template]_Loom_SDI12_Sensor.h"


///////////////////////////////////////////////////////////////////////////////
LoomSDI12SensorTemplate::LoomSDI12SensorTemplate(
		LoomManager* manager,
const char* module_name,
		int				num_samples
	) 
	: LoomSDI12Sensor( module_name, num_samples )
{
	this->module_type = LoomModule::Type::SDI12SensorTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
LoomSDI12SensorTemplate::LoomSDI12SensorTemplate(LoomManager* manager, JsonArrayConst p)
	: LoomSDI12SensorTemplate( EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void LoomSDI12SensorTemplate::print_config()
{
	LoomSDI12Sensor::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void LoomSDI12SensorTemplate::print_state()
{
	LoomSDI12Sensor::print_state();
	// print information about LoomSDI12SensorTemplate state 
}

///////////////////////////////////////////////////////////////////////////////
void LoomSDI12SensorTemplate::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Key1  : ", val1);
	LPrintln("\t", "Key2  : ", val2);
}

///////////////////////////////////////////////////////////////////////////////
void LoomSDI12SensorTemplate::measure()
{
	// measure data from sensor
}

///////////////////////////////////////////////////////////////////////////////
void LoomSDI12SensorTemplate::package(JsonObject json)
{
	package_json(json, module_name, 
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool LoomSDI12SensorTemplate::dispatch(JsonObject json)
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
void LoomSDI12SensorTemplate::calibrate()
{

}

///////////////////////////////////////////////////////////////////////////////
	