
#include "Loom_Relay.h"


// --- CONSTRUCTOR ---
Loom_Relay::Loom_Relay( char* module_name, byte pin ) : LoomActuator( module_name ) 
{
	this->pin = pin;

	pinMode(pin, OUTPUT);
	on = false;
	digitalWrite(pin, LOW);
}

// --- DESTRUCTOR ---
Loom_Relay::~Loom_Relay() 
{

}


// --- PUBLIC METHODS ---

void Loom_Relay::print_config() 
{
	LoomModule::print_config();
}

void Loom_Relay::print_state()
{
	print_module_label();
	Println4('\t', "Relay ", pin, (on) ? " On" : " Off" );
}

void Loom_Relay::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	append_to_bundle(bndl, id_prefix, "Relay", on, NEW_MSG);
}


bool Loom_Relay::message_route(OSCMessage& msg, int address_offset)
{
	if ( msg.fullMatch( "/SetRelay" , address_offset) ) {
		set_relay(msg); return true;
	}

	return false;
}



void Loom_Relay::set_relay(bool state)
{
	on = state;
	digitalWrite(pin, (on) ? HIGH : LOW);

	if (print_verbosity == HIGH) {
		print_module_label();
		Println3("Set relay on pin ", pin, (on) ? " High" : " Low");
	}
}


void Loom_Relay::set_relay(OSCMessage& msg)
{
	// 0 : On / Off
	set_relay( msg.getInt(0) );
}
