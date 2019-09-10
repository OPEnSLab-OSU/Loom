///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Stepper.h
/// @brief		File for the Loom_Stepper definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Loom_Actuator.h"

#include <Adafruit_MotorShield.h>
#include <Adafruit_PWMServoDriver.h>


/// Number of possible steppers.
/// Dependent on stepper controller breakout.
#define NUM_STEPPERS 2


///////////////////////////////////////////////////////////////////////////////


// ### (LoomActuator) | dependencies: [] | conflicts: []
/// Stepper actuator module
// ###
class Loom_Stepper : public LoomActuator
{

protected:

	/// Underlying motor shield controller
	Adafruit_MotorShield*	AFMS;	

	/// Array of stepper controllers
	Adafruit_StepperMotor*	motors[NUM_STEPPERS];		

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	Loom_Stepper();

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_Stepper(JsonArrayConst p);

	/// Destructor
	~Loom_Stepper();

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	bool		dispatch(JsonObject json) override;
	void		add_config(JsonObject json) override;

	/// Move specified stepper specified steps, speed, and direction
	/// \param[in]	motor		Which stepper to move (0-3)
	/// \param[in]	steps		Number of steps to rotate
	///	\param[in]	speed		Speed to move at (0-255)
	/// \param[in]	clockwise 	True to rotate clockwise, false for counterclock-wise
	void		move_steps(uint8_t motor, uint16_t steps, uint8_t speed, bool clockwise);

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;

	/// Override with empty body as no state to print
	void		print_state() override {}

private:

};

