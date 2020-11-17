///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Stepper.cpp
/// @brief		File for Loom_Stepper implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "Stepper.h"

#include <Wire.h>


///////////////////////////////////////////////////////////////////////////////
Loom_Stepper::Loom_Stepper(LoomManager* manager)
	: LoomActuator(manager, "Stepper", Type::Stepper )
{
  LMark;
	AFMS = new Adafruit_MotorShield();
  LMark;
	for (auto i = 0; i < NUM_STEPPERS; i++){
   	LMark;
		motors[i] = AFMS->getStepper(200, i+1);
  	LMark;
	}

	AFMS->begin();
  LMark;

	yield();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_Stepper::Loom_Stepper(LoomManager* manager, JsonArrayConst p)
	: Loom_Stepper(manager) {}

///////////////////////////////////////////////////////////////////////////////
Loom_Stepper::~Loom_Stepper()
{
  LMark;
	delete AFMS;
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Stepper::add_config(JsonObject json)
{
  LMark;
	// add_config_aux(json, module_name,
	// 	module_name, stepper_count
	// );
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Stepper::dispatch(JsonObject json)
{
  LMark;
	JsonArray params = json["params"];
  LMark;
	switch( (char)json["func"] ) {
		case 's': if (params.size() >= 4) { move_steps( EXPAND_ARRAY(params, 4) ); } return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Stepper::move_steps(const uint8_t motor, const uint16_t steps, const uint8_t speed, const bool clockwise)
{
  LMark;
	if (motor < NUM_STEPPERS) {
   	LMark;
		motors[motor]->setSpeed( (speed > 0) ? speed : 0);
   	LMark;
		motors[motor]->step( (steps > 1) ? steps : 0, (clockwise) ? FORWARD : BACKWARD, SINGLE);
   	LMark;
		yield();
  	LMark;

		if (print_verbosity == Verbosity::V_HIGH) {
    	LMark;
			print_module_label();
    	LMark;
			LPrint("Set stepper ", motor, " to move ", steps, " steps ");
    	LMark;
			LPrintln("at speed ", speed, ", direction ", (clockwise) ? "clockwise" : "counterclockwise");
   		LMark;
		}

	}
}

///////////////////////////////////////////////////////////////////////////////
