///////////////////////////////////////////////////////////////////////////////
///
/// @file		Stepper.cpp
/// @brief		File for Stepper implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_ACTUATORS

#include "Stepper.h"
#include "Module_Factory.h"

#include <Wire.h>

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////

REGISTER(LoomModule, Stepper, "Stepper");

///////////////////////////////////////////////////////////////////////////////
Stepper::Stepper() 
	: Actuator("Stepper", Type::Stepper ) 
{
	AFMS = new Adafruit_MotorShield();
	for (auto i = 0; i < NUM_STEPPERS; i++){
		motors[i] = AFMS->getStepper(200, i+1);
	}

	AFMS->begin();
	
	yield();
}

///////////////////////////////////////////////////////////////////////////////
Stepper::Stepper(JsonArrayConst p)
	: Stepper() {} 

///////////////////////////////////////////////////////////////////////////////
Stepper::~Stepper() 
{
	delete AFMS;
}

///////////////////////////////////////////////////////////////////////////////
void Stepper::add_config(JsonObject json)
{
	// add_config_aux(json, module_name,
	// 	module_name, stepper_count
	// );
}

///////////////////////////////////////////////////////////////////////////////
bool Stepper::dispatch(JsonObject json)
{
	JsonArray params = json["params"];
	switch( (char)json["func"] ) {
		case 's': if (params.size() >= 4) { move_steps( EXPAND_ARRAY(params, 4) ); } return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void Stepper::move_steps(const uint8_t motor, const uint16_t steps, const uint8_t speed, const bool clockwise)
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

#endif // ifdef LOOM_INCLUDE_ACTUATORS
