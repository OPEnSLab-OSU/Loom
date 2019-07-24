
#include "Loom_Stepper.h"

#include <Wire.h>


///////////////////////////////////////////////////////////////////////////////
Loom_Stepper::Loom_Stepper( 
		const char*		module_name, 
		uint8_t			stepper_count 
	) 
	: LoomActuator( module_name ) 
{
	this->module_type = LoomModule::Type::Stepper;


	this->stepper_count = stepper_count;
	this->motors = new Adafruit_StepperMotor*[stepper_count];

	AFMS = new Adafruit_MotorShield();
	for (int i = 0; i < stepper_count; i++){
		motors[i] = AFMS->getStepper(200, i+1);
	}

	AFMS->begin();
	
	yield();
}

///////////////////////////////////////////////////////////////////////////////
Loom_Stepper::Loom_Stepper(JsonArrayConst p)
	: Loom_Stepper( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
Loom_Stepper::~Loom_Stepper() 
{
	for (int i = 0; i < stepper_count; i++)
	{
		delete motors[i];
	}

	delete motors;
	delete AFMS;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Stepper::print_config() 
{
	LoomModule::print_config();
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Stepper::dispatch(JsonObject json)
{
	if ( strcmp(json["module"], module_name) == 0 ) {
		JsonArray params = json["params"];
		return functionRoute(
			json["func"],
			"move_steps", [this, params]() { if (params.size() >= 4) { move_steps( EXPAND_ARRAY(params, 4) ); } else { LPrintln("Not enough parameters"); } } 
		);
	} else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Stepper::move_steps(int motor, int steps, int speed, bool clockwise)
{
	motors[motor]->setSpeed( (speed > 0) ? speed : 0);
	motors[motor]->step( (steps > 1) ? steps : 0, (clockwise) ? FORWARD : BACKWARD, SINGLE);
	yield();

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrint("Set stepper ", motor, " to move ", steps, " steps ");
		LPrintln("at speed ", speed, ", direction ", (clockwise) ? "clockwise" : "counterclockwise");
	}
}

///////////////////////////////////////////////////////////////////////////////
