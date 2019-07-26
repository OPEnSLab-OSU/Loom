
#include "Loom_Relay.h"


///////////////////////////////////////////////////////////////////////////////
Loom_Relay::Loom_Relay( 
		const char*		module_name, 
		byte			pin 
	) 
	: LoomActuator( module_name, Type::Relay ) 
	, pin(pin)
	, on(false)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

///////////////////////////////////////////////////////////////////////////////
Loom_Relay::Loom_Relay(JsonArrayConst p)
	: Loom_Relay( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Relay::add_config(JsonObject json)
{
	// add_config_aux(json, module_name,
	// 	module_name, pin
	// );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Relay::print_state()
{
	print_module_label();
	LPrintln("\tRelay ", pin, (on) ? " On" : " Off" );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Relay::package(JsonObject json)
{
	package_json(json, module_name, "state", on);
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Relay::dispatch(JsonObject json)
{
	// if ( strcmp(json["module"], module_name) == 0 ) {
	// 	JsonArray params = json["params"];
	// 	return functionRoute(
	// 		json["func"],
	// 		"set", [this, params]() { if (params.size() >= 1) { set( EXPAND_ARRAY(params, 1) ); } } 
	// 	);
	// } else {
	// 	return false;
	// }

	if ( strcmp(json["module"], module_name) == 0 ) {
		JsonArray params = json["params"];
		switch( (char)json["func"] ) {
			case 's': if (params.size() >= 1) { set( EXPAND_ARRAY(params, 1) ); } return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Relay::set(bool state)
{
	on = state;
	digitalWrite(pin, (on) ? HIGH : LOW);

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("Set relay on pin ", pin, (on) ? " High" : " Low");
	}
}

///////////////////////////////////////////////////////////////////////////////
