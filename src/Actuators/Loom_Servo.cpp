
#include "Loom_Servo.h"

#define SERVOMIN  150	// This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600	// This is the 'maximum' pulse length count (out of 4096)


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_Servo::Loom_Servo( char* module_name, uint8_t servo_count ) : LoomActuator( module_name ) 
{

	this->servo_count = servo_count;

	this->positions = new int[servo_count];

	servo_driver.begin();
	servo_driver.setPWMFreq(60);
}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_Servo::~Loom_Servo() 
{
	delete positions;
}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void Loom_Servo::print_config() 
{
	LoomModule::print_config();
}

/////////////////////////////////////////////////////////////////////
void Loom_Servo::print_state()
{
	print_module_label();
	Println2('\t', "Servo Positions:" );
	for (int i = 1; i < servo_count; i++) {
		Println5('\t\t', "Degree ", i, ": ", positions[i] );
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Servo::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, "Positions");

	char tmp[8];
	for (int i = 0; i < servo_count; i++) {
		sprintf(tmp, "Servo%d", i);
		if (i == 0) {
			append_to_bundle(bndl, id_prefix, tmp, positions[i], NEW_MSG); 
		}
		append_to_bundle(bndl, id_prefix, tmp, positions[i]);
	}
}

/////////////////////////////////////////////////////////////////////
bool Loom_Servo::message_route(OSCMessage& msg, int address_offset)
{
	if ( msg.fullMatch( "/SetServo" , address_offset) ) {
		set_degree(msg); return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////
void Loom_Servo::set_degree(int servo, int degree)
{
	
	if (servo < servo_count) {
		servo_driver.setPWM(servo, 0, map(degree, 0, 180, SERVOMIN, SERVOMAX));
		positions[servo]          = degree;		
	}	

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		Println4("Set servo ", servo, "to degree ", degree);
	}

}

/////////////////////////////////////////////////////////////////////
void Loom_Servo::set_degree(OSCMessage& msg)
{
	// 0 : Servo #
	// 1 : Degree
	set_degree( msg.getInt(0), msg.getInt(1) );
}



