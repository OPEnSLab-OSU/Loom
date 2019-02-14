
#include "Loom_Servo.h"

#define SERVOMIN  150	// This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600	// This is the 'maximum' pulse length count (out of 4096)



// --- CONSTRUCTOR ---
Loom_Servo::Loom_Servo( char* module_name ) : LoomActuator( module_name ) 
{
	servo_driver.begin();
	servo_driver.setPWMFreq(60);
}

// --- DESTRUCTOR ---
Loom_Servo::~Loom_Servo() 
{

}


// --- PUBLIC METHODS ---

void Loom_Servo::print_config() 
{
	LoomModule::print_config();
}

void Loom_Servo::print_state()
{
	print_module_label();
	LOOM_DEBUG_Println2('\t', "Servo Positions:" );
	for (int i = 1; i < NUM_SERVOS; i++) {
		LOOM_DEBUG_Println5('\t\t', "Degree ", i, ": ", positions[i] );
	}
}

void Loom_Servo::package(OSCBundle* bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, "Positions");

	char tmp[8];
	for (int i = 0; i < NUM_SERVOS; i++) {
		sprintf(tmp, "Servo%d", i);
		if (i == 0) {
			append_to_bundle(bndl, id_prefix, tmp, positions[i], NEW_MSG); 
		}
		append_to_bundle(bndl, id_prefix, tmp, positions[i]);
	}
}

void Loom_Servo::set_degree(int servo, int degree)
{
	
	if (servo < NUM_SERVOS) {
		// uint16_t pulse_length = map(degree, 0, 180, SERVOMIN, SERVOMAX);
		servo_driver.setPWM(servo, 0, map(degree, 0, 180, SERVOMIN, SERVOMAX));
		positions[servo]          = degree;		
		// pre_pulselength[servo] = pulse_length;
	}	

}

