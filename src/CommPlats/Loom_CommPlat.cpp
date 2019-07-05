
#include "Loom_CommPlat.h"


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
