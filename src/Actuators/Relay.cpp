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
	: Actuator("Relay", Type::Relay) 
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
	// add_config_aux(json, module_name,
	// 	module_name, pin
	// );
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
// 	LPrintln("Command sent to relay is:");
// 	serializeJsonPretty(json, Serial);

// 	LPrintln("Function char is: ", json["func"].as<const char*>() );
// 	LPrintln("Function char is: ", json["func"].as<char>() );
// 	LPrintln("Function char is: ", json["func"].as<unsigned char>() );


	JsonArray params = json["params"];
	// switch( json["func"].as<const char*>()[0] ) { // works
	// switch( json["func"].as<unsigned char>() ) { 
	switch( (char)json["func"] ) {
		// case 's': if (params.size() >= 1) { set( EXPAND_ARRAY(params, 1) ); } return true;
		case 115: if (params.size() >= 1) { set( EXPAND_ARRAY(params, 1) ); } return true;
	}
	
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void Relay::set(const bool state)
{
	LPrintln("In set");

	on = state;
	digitalWrite(pin, (on) ? HIGH : LOW);

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("Set relay on pin ", pin, (on) ? " High" : " Low");
	}
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_ACTUATORS
