#pragma once

#include "Loom_Actuator.h"

#include <Adafruit_MotorShield.h>
#include <Adafruit_PWMServoDriver.h>


/// Number of possible steppers.
/// Dependent on stepper controller breakout.
#define NUM_STEPPERS 4


///////////////////////////////////////////////////////////////////////////////


// ### (LoomActuator) | dependencies: [] | conflicts: []
/// Stepper actuator module
// ###
class Loom_Stepper : public LoomActuator
{

protected:

	Adafruit_MotorShield*	AFMS;			/// Underlying motor shield controller
	Adafruit_StepperMotor**	motors;			/// Array of stepper controllers
	uint8_t 				stepper_count;	/// Number of steppers

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Stepper module constructor
	///
	/// \param[in]	module_name		String | <"Stepper"> | null | Stepper module name
	/// \param[in]	servo_count		Int | <2> | [1-2] | How many steppers to enable
	Loom_Stepper(
			const char*		module_name		= "Stepper" ,
			uint8_t			stepper_count	= NUM_STEPPERS
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_Stepper(JsonVariant p);

	/// Destructor
	~Loom_Stepper();

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	bool		cmd_route(JsonObject json) override;

	/// Move specified stepper specified steps, speed, and direction
	/// \param[in]	motor		Which stepper to move (0-3)
	/// \param[in]	steps		Number of steps to rotate
	///	\param[in]	speed		Speed to move at (0-255)
	/// \param[in]	clockwise 	True to rotate clockwise, false for counterclock-wise
	void		move_steps(int motor, int steps, int speed, bool clockwise);

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_state() override {}

private:

};

