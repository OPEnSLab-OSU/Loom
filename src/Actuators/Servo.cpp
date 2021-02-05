///////////////////////////////////////////////////////////////////////////////
///
/// @file		Servo.cpp
/// @brief		File for Servo implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_ACTUATORS

#include "Servo.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////

#define SERVOMIN  150	///< This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600	///< This is the 'maximum' pulse length count (out of 4096)


///////////////////////////////////////////////////////////////////////////////
Servo::Servo()
	: Actuator("Servo")
	, positions{}
{
  LMark;
	servo_driver.begin();
  LMark;
	servo_driver.setPWMFreq(60);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Servo::Servo(JsonArrayConst p)
	: Servo() {}

///////////////////////////////////////////////////////////////////////////////
void Servo::print_state() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("\tServo Positions:" );
  LMark;
	for (int i = 0; i < NUM_SERVOS; i++) {
   	LMark;
		LPrintln("\t\tDegree ", i, ": ", positions[i] );
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Servo::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;
	char buf[8];
  LMark;
	for (auto i = 0; i < NUM_SERVOS; i++) {
   	LMark;
		sprintf(buf, "Servo%d", i);
   	LMark;
		data[buf] = positions[i];
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Servo::dispatch(JsonObject json)
{
  LMark;
	JsonArray params = json["params"];
  LMark;
	switch( (char)json["func"] ) {
		case 's': if (params.size() >= 2) { set_degree( EXPAND_ARRAY(params, 2) ); } return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void Servo::set_degree(const uint8_t servo, const uint8_t degree)
{
  LMark;
	if (servo < NUM_SERVOS) {
   	LMark;
		servo_driver.setPWM(servo, 0, map(degree, 0, 180, SERVOMIN, SERVOMAX));
   	LMark;
		positions[servo]          = degree;
  	LMark;
	}

	if (print_verbosity == Verbosity::V_HIGH) {
   	LMark;
		print_module_label();
   	LMark;
		LPrintln("Set servo ", servo, "to degree ", degree);
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_ACTUATORS
