///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Relay.cpp
/// @brief		File for Loom_Relay implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "Relay.h"


///////////////////////////////////////////////////////////////////////////////
Loom_Relay::Loom_Relay( 
		LoomManager* 	manager,
		const byte					pin 
	) 
	: LoomActuator(manager, "Relay", Type::Relay ) 
	, pin(pin)
	, on(false)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

///////////////////////////////////////////////////////////////////////////////
Loom_Relay::Loom_Relay(LoomManager* manager, JsonArrayConst p)
	: Loom_Relay(manager, (byte)p[0] ) {}
// explicitly cast because with only one parameter, JsonVariant p[0] can 
// implicitly cast to either JsonArrayConst or byte of regular constructor


///////////////////////////////////////////////////////////////////////////////
void Loom_Relay::add_config(JsonObject json)
{
	// add_config_aux(json, module_name,
	// 	module_name, pin
	// );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Relay::print_state() const
{
	print_module_label();
	LPrintln("\tRelay ", pin, (on) ? " On" : " Off" );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Relay::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	data["on"] = on;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Relay::dispatch(JsonObject json)
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
void Loom_Relay::set(const bool state)
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
