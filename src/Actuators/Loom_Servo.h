
#ifndef LOOM_SERVO_h
#define LOOM_SERVO_h

#include "Loom_Actuator.h"

#include <Adafruit_PWMServoDriver.h>


#define NUM_SERVOS 8



class Loom_Servo : public LoomActuator
{

protected:

	/// Underlying servo driver instance
	Adafruit_PWMServoDriver servo_driver = Adafruit_PWMServoDriver();

	/// Number of servos
	uint8_t		servo_count;

	/// Positions of the servos
	int*		positions;

public:

	// --- CONSTRUCTOR ---
	Loom_Servo( 
			char*		module_name		=  "Servo",  //  SERVO_Module_Name,

			uint8_t		servo_count		= NUM_SERVOS  //  SERVO_Servo_Count 
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_Servo();

	// Inherited (Overriding) Methods
	void		print_config();
	void		print_state();
	void		package(OSCBundle& bndl, char* suffix="");
	bool		message_route(OSCMessage& msg, int address_offset);


	// --- Actuator Control ---

	/// Set servo position.
	/// \param[in]	servo		The servo number to control 
	/// \param[in]	degree		The position to set the servo to
	void		set_degree(int servo, int degree);

	/// Set servo position.
	/// Settings enclosed in message, forwards to set_degree with int args
	/// \param[in]	msg		The message to parse
	void		set_degree(OSCMessage& msg);	

private:

};


#endif // of LOOM_SERVO_h
