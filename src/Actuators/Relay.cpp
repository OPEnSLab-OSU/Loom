///////////////////////////////////////////////////////////////////////////////
///
/// @file		Relay.cpp
/// @brief		File for Relay implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_ACTUATORS

#include "Relay.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
Relay::Relay(const byte pin)
	: Actuator("Relay") 
	, pin(pin)
	, on(false)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

///////////////////////////////////////////////////////////////////////////////
Relay::Relay(JsonArrayConst p)
	: Relay((byte)p[0] ) {}
// explicitly cast because with only one parameter, JsonVariant p[0] can 
// implicitly cast to either JsonArrayConst or byte of regular constructor


///////////////////////////////////////////////////////////////////////////////
void Relay::add_config(JsonObject json)
{
	JsonArray params = add_config_temp(json, module_name);
	params.add(pin);
}

///////////////////////////////////////////////////////////////////////////////
void Relay::print_state() const
{
	print_module_label();
	LPrintln("\tRelay ", pin, (on) ? " On" : " Off" );
}

///////////////////////////////////////////////////////////////////////////////
void Relay::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	data["on"] = on;
}

///////////////////////////////////////////////////////////////////////////////
bool Relay::dispatch(JsonObject json)
{
	JsonArray params = json["params"];
	switch( (char)json["func"] ) {
		case 's': if (params.size() >= 1) { set( EXPAND_ARRAY(params, 1) ); } return true;
	}
	
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void Relay::set(const bool state)
{
	on = state;
	digitalWrite(pin, (on) ? HIGH : LOW);

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("Set relay on pin ", pin, (on) ? " High" : " Low");
	}
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_ACTUATORS
