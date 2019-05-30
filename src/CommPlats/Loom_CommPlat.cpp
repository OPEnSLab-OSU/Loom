
#include "Loom_CommPlat.h"



union data_value { // Used in translation between OSC and strings
	int32_t  i;
	float    f;
	uint32_t u;
};



/////////////////////////////////////////////////////////////////////
char* LoomCommPlat::enum_comm_plat_string(CommPlatform c)
{
	switch(c) {
		case CommPlatform::WIFI : return "WiFi";
		case CommPlatform::LORA : return "LoRa";
		case CommPlatform::NRF  : return "nRF";
		case CommPlatform::SSER : return "SlipSerial";	    
	}
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
LoomCommPlat::LoomCommPlat( 
		const char*		module_name, 
		uint			max_message_len 
	) 
	: LoomModule( module_name )
{
	this->max_message_len = max_message_len;
	this->signal_strength = 0;
}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
LoomCommPlat::~LoomCommPlat()
{

}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void LoomCommPlat::print_config() 
{
	LoomModule::print_config();

	LPrintln('\t', "Max Message Length  : ", max_message_len );
}



/////////////////////////////////////////////////////////////////////

void LoomCommPlat::package(JsonObject json)
{

}