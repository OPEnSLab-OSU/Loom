///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Servo.cpp
/// @brief		File for Loom_Servo implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "Loom_Servo.h"

#define SERVOMIN  150	///< This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600	///< This is the 'maximum' pulse length count (out of 4096)


///////////////////////////////////////////////////////////////////////////////
Loom_Servo::Loom_Servo()
	: LoomActuator( "Servo", Type::Servo )
	, positions{}
{
	servo_driver.begin();
	servo_driver.setPWMFreq(60);
}

///////////////////////////////////////////////////////////////////////////////
Loom_Servo::Loom_Servo(JsonArrayConst p)
	: Loom_Servo() {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Servo::add_config(JsonObject json)
{
	// add_config_aux(json, module_name,
	// 	module_name, servo_count
	// );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Servo::print_state() const 
{
	print_module_label();
	LPrintln("\tServo Positions:" );
	for (int i = 0; i < NUM_SERVOS; i++) {
		LPrintln("\t\tDegree ", i, ": ", positions[i] );
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Servo::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	char buf[8];
	for (auto i = 0; i < NUM_SERVOS; i++) {
		sprintf(buf, "Servo%d", i);
		data[buf] = positions[i];
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Servo::dispatch(JsonObject json)
{
	JsonArray params = json["params"];
	switch( (char)json["func"] ) {
		case 's': if (params.size() >= 2) { set_degree( EXPAND_ARRAY(params, 2) ); } return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Servo::set_degree(const uint8_t servo, const uint8_t degree)
{	
	if (servo < NUM_SERVOS) {
		servo_driver.setPWM(servo, 0, map(degree, 0, 180, SERVOMIN, SERVOMAX));
		positions[servo]          = degree;
	}

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("Set servo ", servo, "to degree ", degree);
	}
}

///////////////////////////////////////////////////////////////////////////////

void Loom_Servo::Run() {
	int argNum = 2;
	var args[argNum];
	int i = 0;

	for(auto dependency : Dependencies) {
		for(auto value : dependency->Values) {
			if(i < argNum) {
				args[i] = value;
				i++;
			}
		}
	}

	set_degree(
		args[0].retrieve<uint8_t>().value_or(0),
		args[1].retrieve<uint8_t>().value_or(0));
}

///////////////////////////////////////////////////////////////////////////////

