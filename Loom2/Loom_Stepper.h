
#ifndef LOOM_STEPPER_h
#define LOOM_STEPPER_h

#include "Loom_Actuator.h"

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_PWMServoDriver.h>


#define NUM_STEPPERS 4


class Loom_Stepper : public LoomActuator
{

protected:

	Adafruit_MotorShield  *AFMS;
	Adafruit_StepperMotor *motors[NUM_STEPPERS];

public:

	// --- CONSTRUCTOR ---
	Loom_Stepper( char* module_name );

	// --- DESTRUCTOR ---
	virtual ~Loom_Stepper();

	void print_config();
	void print_state() {}
	void measure() {}
	void package(OSCBundle* bndl, char* suffix="") {}

	void move_steps(int motor, uint steps, uint speed, bool clockwise);

private:

};


#endif