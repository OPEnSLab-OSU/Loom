
#include "[Template]_Loom_Sensor.h"


///////////////////////////////////////////////////////////////////////////////
LoomSensorTemplate::LoomSensorTemplate(
		LoomManager* manager,
const char* module_name, 
		int 			num_samples
	) 
	: LoomSensor( module_name, num_samples )
{
	this->module_type = LoomModule::Type::SensorTemplateType;

	// Initialize member variables
}

///////////////////////////////////////////////////////////////////////////////
LoomSensorTemplate::LoomSensorTemplate(LoomManager* manager, JsonArrayConst p)
	: LoomSensorTemplate( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void LoomSensorTemplate::print_config()
{
	LoomSensor::print_config();
	LPrintln('\t', "Description         : ", "value to print" );
}

///////////////////////////////////////////////////////////////////////////////
void LoomSensorTemplate::print_state()
{
	LoomSensor::print_state();
	// print information about LoomSensorTemplate state 
}

///////////////////////////////////////////////////////////////////////////////
void LoomSensorTemplate::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Key1  : ", val1);
	LPrintln("\t", "Key2  : ", val2);
}

///////////////////////////////////////////////////////////////////////////////
void LoomSensorTemplate::measure()
{
	// measure data from sensor
}

///////////////////////////////////////////////////////////////////////////////
void LoomSensorTemplate::package(JsonObject json)
{
	package_json(json, module_name, 
		"key1",		"val1",
		"key2",		"val2"
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

///////////////////////////////////////////////////////////////////////////////
bool LoomSensorTemplate::dispatch(JsonObject json)
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
void LoomSensorTemplate::calibrate()
{

}

///////////////////////////////////////////////////////////////////////////////
	