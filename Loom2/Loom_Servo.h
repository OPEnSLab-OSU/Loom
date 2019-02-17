
#ifndef LOOM_SERVO_h
#define LOOM_SERVO_h

#include "Loom_Actuator.h"

#include <Adafruit_PWMServoDriver.h>


#define NUM_SERVOS 8


class Loom_Servo : public LoomActuator
{

protected:

	Adafruit_PWMServoDriver servo_driver = Adafruit_PWMServoDriver();

	int 	positions[NUM_SERVOS];
	// double 	pre_pulselength[NUM_SERVOS]

public:

	// --- CONSTRUCTOR ---
	Loom_Servo( char* module_name );

	// --- DESTRUCTOR ---
	virtual ~Loom_Servo();

	// --- PUBLIC METHODS ---

	void print_config();
	void print_state();
	void measure() {}
	void package(OSCBundle& bndl, char* suffix="");

	void set_degree(int servo, int degree);

private:

};


#endif
