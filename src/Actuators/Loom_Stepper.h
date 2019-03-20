
#ifndef LOOM_STEPPER_h
#define LOOM_STEPPER_h

#include "Loom_Actuator.h"

#include <Adafruit_MotorShield.h>
#include <Adafruit_PWMServoDriver.h>



// #define NUM_STEPPERS 4


class Loom_Stepper : public LoomActuator
{

protected:

	/// Underlying motor shield controller
	Adafruit_MotorShield*	AFMS;

	/// Array of stepper controllers
	Adafruit_StepperMotor**	motors;

	/// Number of steppers
	uint8_t 				stepper_count;

public:

	// --- CONSTRUCTOR ---
	Loom_Stepper( 
			char*		module_name		= STEPPER_Module_Name, // "Stepper" ,

			uint8_t		stepper_count	= STEPPER_Stepper_Count // NUM_STEPPERS
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_Stepper();

	// Inherited (Overriding) Methods
	void		print_config();
	void		print_state() {}
	void		package(OSCBundle& bndl, char* suffix="") {}
	bool		message_route(OSCMessage& msg, int address_offset);


	// --- Actuator Control ---
	/// Move specified stepper specified steps, speed, and direction
	/// \param[in]	motor		Which stepper to move (0-3)
	/// \param[in]	steps		Number of steps to rotate
	///	\param[in]	speed		Speed to move at (0-255)
	/// \param[in]	clockwise 	True to rotate clockwise, false for counterclock-wise
	void		move_steps(int motor, int steps, int speed, bool clockwise);

	/// Move stepper.
	/// Settings enclosed in message, forwards to move_steps with int args
	void		move_steps(OSCMessage& msg);

private:

};


#endif // of LOOM_STEPPER_h
