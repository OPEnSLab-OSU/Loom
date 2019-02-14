
#include "Loom_Stepper.h"


// --- CONSTRUCTOR ---
Loom_Stepper::Loom_Stepper( char* module_name ) : LoomActuator( module_name ) 
{
	AFMS = new Adafruit_MotorShield();
	for (int i = 0; i < NUM_STEPPERS; i++){
		motors[i] = AFMS->getStepper(200, i+1);
	}

	AFMS->begin();

	for (int i = 0; i < NUM_STEPPERS; i++){
		motors[i]->setSpeed(50);
	}
	
	yield();
	// set_stepper_steps(0,0,60,50);
	// set_stepper_steps(1,0,60,50);
}

// --- DESTRUCTOR ---
Loom_Stepper::~Loom_Stepper() 
{

}


// --- PUBLIC METHODS ---

void Loom_Stepper::print_config() 
{
	LoomModule::print_config();
}

void Loom_Stepper::print_state()
{
	print_module_label();
	// LOOM_DEBUG_Println3('\t', "Relay Pin           : ", pin );
}

void Loom_Stepper::package(OSCBundle* bndl, char* suffix)
{
	// char id_prefix[30]; 
	// resolve_bundle_address(id_prefix, "Positions");

	// char tmp[8];
	// for (int i = 0; i < NUM_SERVOS; i++) {
	// 	sprintf(tmp, "Stepper%d", i);
	// 	append_to_bundle_msg_key_value(bndl, tmp, positions[i]);
	// }
}



void Loom_Stepper::move_steps(int motor, uint steps, uint speed, bool clockwise)
{
	//int set_degree = map(degree,0,360,0,200);
	motors[motor]->setSpeed( (speed > 0) ? speed : 0);
	motors[motor]->step(steps, (clockwise) ? FORWARD : BACKWARD, SINGLE);
	yield();
}
