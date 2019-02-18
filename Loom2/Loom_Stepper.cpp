
#include "Loom_Stepper.h"


// --- CONSTRUCTOR ---
Loom_Stepper::Loom_Stepper( char* module_name, uint8_t stepper_count ) : LoomActuator( module_name ) 
{
	this->stepper_count = stepper_count;
	this->motors = new Adafruit_StepperMotor*[stepper_count];

	AFMS = new Adafruit_MotorShield();
	for (int i = 0; i < stepper_count; i++){
		motors[i] = AFMS->getStepper(200, i+1);
	}

	AFMS->begin();
	
	yield();
}

// --- DESTRUCTOR ---
Loom_Stepper::~Loom_Stepper() 
{
	for (int i = 0; i < stepper_count; i++)
	{
		delete motors[i];
	}

	delete motors;
	delete AFMS;
}


// --- PUBLIC METHODS ---

void Loom_Stepper::print_config() 
{
	LoomModule::print_config();
}

void Loom_Stepper::move_steps(int motor, int steps, int speed, bool clockwise)
{
	motors[motor]->setSpeed( (speed > 0) ? speed : 0);
	motors[motor]->step( (steps > 1) ? steps : 0, (clockwise) ? FORWARD : BACKWARD, SINGLE);
	yield();

	if (print_verbosity == HIGH) {
		print_module_label();
		LOOM_DEBUG_Print5("Set stepper ", motor, " to move ", steps, " steps ");
		LOOM_DEBUG_Println4("at speed ", speed, ", direction ", (clockwise) ? "clockwise" : "counterclockwise");
	}

}
