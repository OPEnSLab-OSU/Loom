
#include "Loom_Relay.h"


///////////////////////////////////////////////////////////////////////////////
Loom_Relay::Loom_Relay( 
		byte			pin 
	) 
	: LoomActuator( "Relay", Type::Relay ) 
	, pin(pin)
	, on(false)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

///////////////////////////////////////////////////////////////////////////////
Loom_Relay::Loom_Relay(JsonArrayConst p)
	: Loom_Relay( (byte)p[0] ) {}
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
void Loom_Relay::print_state()
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
