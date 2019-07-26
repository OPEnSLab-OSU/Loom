
#include "Loom_Servo.h"

/// This is the 'minimum' pulse length count (out of 4096)
#define SERVOMIN  150	
/// This is the 'maximum' pulse length count (out of 4096)
#define SERVOMAX  600	


///////////////////////////////////////////////////////////////////////////////
Loom_Servo::Loom_Servo( 
		const char* 	module_name
	) 
	: LoomActuator( module_name, Type::Servo ) 
	, positions{}
{
	servo_driver.begin();
	servo_driver.setPWMFreq(60);
}

///////////////////////////////////////////////////////////////////////////////
Loom_Servo::Loom_Servo(JsonArrayConst p)
	: Loom_Servo( (const char*)p[0] ) {} 
// explicitly cast because with only one parameter, JsonVariant p[0] can 
// implicitly cast to either JsonArrayConst or const char* of regular constructor

///////////////////////////////////////////////////////////////////////////////
void Loom_Servo::add_config(JsonObject json)
{
	// add_config_aux(json, module_name,
	// 	module_name, servo_count
	// );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Servo::print_config() 
{
	LoomModule::print_config();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Servo::print_state()
{
	print_module_label();
	LPrintln("\tServo Positions:" );
	for (int i = 0; i < NUM_SERVOS; i++) {
		LPrintln("\t\tDegree ", i, ": ", positions[i] );
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Servo::package(JsonObject json)
{
	char tmp[8];
	for (auto i = 0; i < NUM_SERVOS; i++) {
		sprintf(tmp, "Servo%d", i);
		package_json(json, module_name, tmp, positions[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Servo::dispatch(JsonObject json)
{
	// if ( strcmp(json["module"], module_name) == 0 ) {
	// 	JsonArray params = json["params"];
	// 	return functionRoute(
	// 		json["func"],
	// 		"set_degree", [this, params]() { if (params.size() >= 2) { set_degree( EXPAND_ARRAY(params, 2) ); } } 
	// 	);
	// } else {
	// 	return false;
	// }
	if ( strcmp(json["module"], module_name) == 0 ) {
		JsonArray params = json["params"];
		switch( (char)json["func"] ) {
			case 's': if (params.size() >= 2) { set_degree( EXPAND_ARRAY(params, 2) ); } return true;
		}
	}
	return false;

}

///////////////////////////////////////////////////////////////////////////////
void Loom_Servo::set_degree(uint8_t servo, uint8_t degree)
{	
	if (servo < NUM_SERVOS) {
		servo_driver.setPWM(servo, 0, map(degree, 0, 180, SERVOMIN, SERVOMAX));
		positions[servo]          = degree;		
	}	

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("Set servo ", servo, "to degree ", degree);
	}
}

///////////////////////////////////////////////////////////////////////////////

