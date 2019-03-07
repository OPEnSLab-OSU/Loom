
#ifndef LOOM_STEPPER_h
#define LOOM_STEPPER_h

#include "Loom_Actuator.h"

#include <Adafruit_MotorShield.h>
#include <Adafruit_PWMServoDriver.h>



#define NUM_STEPPERS 4


class Loom_Stepper : public LoomActuator
{

protected:

	/// Underlying motor shield controller
	Adafruit_MotorShield  *AFMS;

	/// Array of stepper controllers
	Adafruit_StepperMotor **motors;

	/// Number of steppers
	uint8_t stepper_count;

public:

	// --- CONSTRUCTOR ---
	Loom_Stepper( char* module_name = "Stepper" ,

					uint8_t stepper_count = NUM_STEPPERS
				);

	// --- DESTRUCTOR ---
	virtual ~Loom_Stepper();

	// General
	void print_config();
	void print_state() {}
	void package(OSCBundle& bndl, char* suffix="") {}
	bool message_route(OSCMessage& msg, int address_offset);

	// Actuator Control 
	void move_steps(int motor, int steps, int speed, bool clockwise);
	void move_steps(OSCMessage& msg);

private:

};


#endif // of LOOM_STEPPER_h
