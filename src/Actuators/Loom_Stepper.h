#pragma once

#include "Loom_Actuator.h"

#include <Adafruit_MotorShield.h>
#include <Adafruit_PWMServoDriver.h>


/// Number of possible steppers.
/// Dependent on stepper controller breakout.
#define NUM_STEPPERS 4


// ### (LoomActuator) | dependencies: [] | conflicts: []
/// Stepper actuator module
// ###
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

	/// Stepper module constructor
	///
	/// \param[in]	module_name		String | <"Stepper"> | null | Stepper module name
	/// \param[in]	servo_count		Int | <2> | [1-2] | How many steppers to enable
	Loom_Stepper(
			const char*		module_name		= "Stepper" ,
			uint8_t			stepper_count	= NUM_STEPPERS
		);

	Loom_Stepper(JsonVariant p);



	/// Destructor
	virtual ~Loom_Stepper();

	// Inherited (Overriding) Methods
	void		print_config() override;
	void		print_state() {}
	bool		cmd_route(JsonObject json);


	// --- Actuator Control ---
	/// Move specified stepper specified steps, speed, and direction
	/// \param[in]	motor		Which stepper to move (0-3)
	/// \param[in]	steps		Number of steps to rotate
	///	\param[in]	speed		Speed to move at (0-255)
	/// \param[in]	clockwise 	True to rotate clockwise, false for counterclock-wise
	void		move_steps(int motor, int steps, int speed, bool clockwise);

private:

};

