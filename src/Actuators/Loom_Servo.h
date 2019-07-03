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

	/// Underlying servo driver instance
	Adafruit_PWMServoDriver servo_driver;// = Adafruit_PWMServoDriver();

	/// Number of servos
	uint8_t		servo_count;

	/// Positions of the servos
	int*		positions;

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Servo module constructor
	///
	/// \param[in]	module_name		String | <"Servo"> | null | Servo module name
	/// \param[in]	servo_count		Int | <8> | [1-8] | How many servos to enable
	Loom_Servo(
			const char*		module_name		= "Servo",
			uint8_t			servo_count		= NUM_SERVOS
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_Servo(JsonVariant p);

	/// Destructor
	~Loom_Servo();

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		package(JsonObject json) override;
	bool		cmd_route(JsonObject json) override;

	/// Set servo position.
	/// \param[in]	servo		The servo number to control
	/// \param[in]	degree		The position to set the servo to
	void		set_degree(int servo, int degree);

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_state() override;

private:

};

