///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Servo.h
/// @brief		File for Loom_Servo definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Loom_Actuator.h"

#include <Adafruit_PWMServoDriver.h>


/// Number of possible servos.
/// Dependent on servo controller breakout.
#define NUM_SERVOS 8


///////////////////////////////////////////////////////////////////////////////


// ### (LoomActuator) | dependencies: [] | conflicts: []
/// Servo actuator module
// ###
class Loom_Servo : public LoomActuator
{

protected:

	Adafruit_PWMServoDriver servo_driver;	///< Underlying servo driver instance

	uint8_t		positions[NUM_SERVOS];		///< Positions of the servos

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	Loom_Servo();

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_Servo(JsonArrayConst p);

	/// Destructor
	~Loom_Servo() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		package(JsonObject json) override;
	bool		dispatch(JsonObject json) override;
	void		add_config(JsonObject json) override;

	/// Set servo position.
	/// \param[in]	servo		The servo number to control
	/// \param[in]	degree		The position to set the servo to
	void		set_degree(uint8_t servo, uint8_t degree);

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_state() override;

private:

};

