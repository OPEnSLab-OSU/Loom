
#ifndef LOOM_SERVO_h
#define LOOM_SERVO_h

#include "Loom_Actuator.h"

#include <Adafruit_PWMServoDriver.h>


#define NUM_SERVOS 8



class Loom_Servo : public LoomActuator
{

protected:

	Adafruit_PWMServoDriver servo_driver = Adafruit_PWMServoDriver();

	uint8_t servo_count;
	int* positions;

public:

	// --- CONSTRUCTOR ---
	Loom_Servo( char* module_name = "Servo",

				uint8_t servo_count = NUM_SERVOS

				);

	// --- DESTRUCTOR ---
	virtual ~Loom_Servo();

	// General
	void print_config();
	void print_state();
	void measure() {}
	void package(OSCBundle& bndl, char* suffix="");
	bool message_route(OSCMessage* msg, int address_offset){}

	// Actuator Control
	void set_degree(int servo, int degree);

private:

};


#endif
