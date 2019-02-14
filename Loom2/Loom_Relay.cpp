
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
	LOOM_DEBUG_Println3('\t', "Relay Pin           : ", pin );
}

void Loom_Relay::package(OSCBundle* bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	append_to_bundle_key_value(bndl, id_prefix, "Relay" , on);
}