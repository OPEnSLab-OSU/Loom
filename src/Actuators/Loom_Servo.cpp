
#include "Loom_Servo.h"

/// This is the 'minimum' pulse length count (out of 4096)
#define SERVOMIN  150	
/// This is the 'maximum' pulse length count (out of 4096)
#define SERVOMAX  600	


///////////////////////////////////////////////////////////////////////////////
Loom_Servo::Loom_Servo( 
		const char* 	module_name, 
		uint8_t 		servo_count 
	) 
	: LoomActuator( module_name ) 
	, servo_count(servo_count)
{
	this->module_type = LoomModule::Type::Servo;

	this->positions = new int[servo_count];

	// this->servo_driver = new Adafruit_PWMServoDriver();

	servo_driver.begin();
	servo_driver.setPWMFreq(60);
}

///////////////////////////////////////////////////////////////////////////////
Loom_Servo::Loom_Servo(JsonArrayConst p)
	: Loom_Servo( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
Loom_Servo::~Loom_Servo() 
{
	delete positions;
	// delete servo_driver;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Servo::add_config(JsonObject json)
{
	add_config_aux(json, module_name,
		module_name, servo_count
	);
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
	LPrintln('\t', "Servo Positions:" );
	for (int i = 0; i < servo_count; i++) {
		LPrintln('\t\t', "Degree ", i, ": ", positions[i] );
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Servo::package(JsonObject json)
{
	char tmp[8];
	for (int i = 0; i < servo_count; i++) {
		sprintf(tmp, "Servo%d", i);
		package_json(json, module_name, tmp, positions[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Servo::dispatch(JsonObject json)
{
	if ( strcmp(json["module"], module_name) == 0 ) {
		JsonArray params = json["params"];
		return functionRoute(
			json["func"],
			"set_degree", [this, params]() { if (params.size() >= 2) { set_degree( EXPAND_ARRAY(params, 2) ); } else { LPrintln("Not enough parameters"); } } 
		);
	} else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Servo::set_degree(int servo, int degree)
{
	
	if (servo < servo_count) {
		servo_driver.setPWM(servo, 0, map(degree, 0, 180, SERVOMIN, SERVOMAX));
		positions[servo]          = degree;		
	}	

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("Set servo ", servo, "to degree ", degree);
	}

}

///////////////////////////////////////////////////////////////////////////////

