#pragma once

#include "Loom_Actuator.h"

#include <Adafruit_PWMServoDriver.h>


/// Number of possible servos.
/// Dependent on servo controller breakout.
#define NUM_SERVOS 8

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

	/// Servo module constructor
	///
	/// \param[in]	module_name		String | <"Servo"> | null | Servo module name
	/// \param[in]	servo_count		Int | <8> | [1-8] | How many servos to enable
	Loom_Servo(
			const char*		module_name		= "Servo",
			uint8_t			servo_count		= NUM_SERVOS
		);

	Loom_Servo(JsonVariant p);



	// --- DESTRUCTOR ---
	virtual ~Loom_Servo();

	// Inherited (Overriding) Methods
	void		print_config() override;
	void		print_state();
	void		package(JsonObject json);
	// bool		message_route(OSCMessage& msg, int address_offset);
	bool		cmd_route(JsonObject);


	// --- Actuator Control ---

	/// Set servo position.
	/// \param[in]	servo		The servo number to control
	/// \param[in]	degree		The position to set the servo to
	void		set_degree(int servo, int degree);

	/// Set servo position.
	/// Settings enclosed in message, forwards to set_degree with int args
	/// \param[in]	msg		The message to parse
	// void		set_degree(OSCMessage& msg);

private:

};


#pragma once