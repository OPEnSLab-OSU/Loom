
#include "Loom_Stepper.h"

#include <Wire.h>


///////////////////////////////////////////////////////////////////////////////
Loom_Stepper::Loom_Stepper() 
	: LoomActuator( "Stepper", Type::Stepper ) 
{
	AFMS = new Adafruit_MotorShield();
	for (auto i = 0; i < NUM_STEPPERS; i++){
		motors[i] = AFMS->getStepper(200, i+1);
	}

	AFMS->begin();
	
	yield();
}

///////////////////////////////////////////////////////////////////////////////
Loom_Stepper::Loom_Stepper(JsonArrayConst p)
	: Loom_Stepper() {} 

///////////////////////////////////////////////////////////////////////////////
Loom_Stepper::~Loom_Stepper() 
{
	delete AFMS;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Stepper::add_config(JsonObject json)
{
	// add_config_aux(json, module_name,
	// 	module_name, stepper_count
	// );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Stepper::print_config() 
{
	LoomModule::print_config();
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Stepper::dispatch(JsonObject json)
{
	// if ( strcmp(json["module"], module_name) == 0 ) {
	// 	JsonArray params = json["params"];
	// 	return functionRoute(
	// 		json["func"],
	// 		"move_steps", [this, params]() { if (params.size() >= 4) { move_steps( EXPAND_ARRAY(params, 4) ); } } 
	// 	);
	// } else {
	// 	return false;
	// }

	if ( strcmp(json["module"], module_name) == 0 ) {
		JsonArray params = json["params"];
		switch( (char)json["func"] ) {
			case 's': if (params.size() >= 4) { move_steps( EXPAND_ARRAY(params, 4) ); } return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Stepper::move_steps(uint8_t motor, uint16_t steps, uint8_t speed, bool clockwise)
{
	if (motor < NUM_STEPPERS) {
		motors[motor]->setSpeed( (speed > 0) ? speed : 0);
		motors[motor]->step( (steps > 1) ? steps : 0, (clockwise) ? FORWARD : BACKWARD, SINGLE);
		yield();

		if (print_verbosity == Verbosity::V_HIGH) {
			print_module_label();
			LPrint("Set stepper ", motor, " to move ", steps, " steps ");
			LPrintln("at speed ", speed, ", direction ", (clockwise) ? "clockwise" : "counterclockwise");
		}

	}
}

///////////////////////////////////////////////////////////////////////////////
