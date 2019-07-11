
#include "Loom_CommPlat.h"
#include "Loom_Manager.h"


///////////////////////////////////////////////////////////////////////////////
const char* LoomCommPlat::enum_comm_plat_string(Platform c)
{
	switch(c) {
		case Platform::WIFI : return "WiFi";
		case Platform::LORA : return "LoRa";
		case Platform::NRF  : return "nRF";
		case Platform::SSER : return "SlipSerial";	    
	}
}

///////////////////////////////////////////////////////////////////////////////
LoomCommPlat::LoomCommPlat( 
		const char*		module_name, 
		uint			max_message_len 
	) 
	: LoomModule( module_name )
{
	this->max_message_len = max_message_len;
	this->signal_strength = 0;
}

///////////////////////////////////////////////////////////////////////////////
void LoomCommPlat::print_config() 
{
	LoomModule::print_config();

	LPrintln('\t', "Max Message Length  : ", max_message_len );
}

///////////////////////////////////////////////////////////////////////////////
bool LoomCommPlat::receive()
{
	if (device_manager != nullptr) {
		// Loom_Manager's json needs to be cleared (passing true to internal_json)
		// in order to copy over correctly
		return receive( device_manager->internal_json(true) );
	} 
	return false;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomCommPlat::send()
{
	if (device_manager != nullptr) {
		JsonObject tmp = device_manager->internal_json();
		if (strcmp(tmp["type"], "data") == 0 ) {
			return send(tmp);
		}
	} 
	return false;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomCommPlat::send(uint16_t destination)
{
	if (device_manager != nullptr) {
		JsonObject tmp = device_manager->internal_json();
		if (strcmp(tmp["type"], "data") == 0 ) {
			return send(tmp, destination);
		}
	} 
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void LoomCommPlat::broadcast()
{
	if (device_manager != nullptr) {
		JsonObject tmp = device_manager->internal_json();
		if (strcmp(tmp["type"], "data") == 0 ) {
			broadcast(tmp);
		}
	} 
}

