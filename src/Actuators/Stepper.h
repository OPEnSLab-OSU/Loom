///////////////////////////////////////////////////////////////////////////////
///
/// @file		Stepper.h
/// @brief		File for Stepper definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_ACTUATORS
#pragma once

#include "Actuator.h"

#include <Adafruit_MotorShield.h>
#include <Adafruit_PWMServoDriver.h>

namespace Loom {

///////////////////////////////////////////////////////////////////////////////

/// Number of possible steppers.
/// Dependent on stepper controller breakout.
#define NUM_STEPPERS 2


///////////////////////////////////////////////////////////////////////////////
///
/// Stepper actuator module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___stepper.html)
/// - [Product Page: Adafruit Stepper Feather](https://www.adafruit.com/product/2927)
/// - [Dependency: Adafruit-PWM-Servo-Driver](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)
/// - [Dependency: Adafruit_Motor_Shield_V2_Library](https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library)
/// - [Datasheet: Motor Driver](https://cdn-shop.adafruit.com/datasheets/TB6612FNG_datasheet_en_20121101.pdf)
/// - [Product Page: Adafruit Stepper Motor](https://www.adafruit.com/product/324)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#stepper-motors-dc-motor--stepper-featherwing)
///
///////////////////////////////////////////////////////////////////////////////
class Stepper : public Actuator
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
	Stepper();

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Stepper(JsonArrayConst p);

	/// Destructor
	~Stepper();

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	bool		dispatch(JsonObject json) override;

	/// Move specified stepper specified steps, speed, and direction
	/// @param[in]	motor		Which stepper to move (0-3)
	/// @param[in]	steps		Number of steps to rotate
	///	@param[in]	speed		Speed to move at (0-255)
	/// @param[in]	clockwise 	True to rotate clockwise, false for counterclock-wise
	void		move_steps(const uint8_t motor, const uint16_t steps, const uint8_t speed, const bool clockwise);

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	/// Override with empty body as no state to print
	void		print_state() const override {}

private:

};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, Stepper, "Stepper");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_ACTUATORS
