
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
	// LPrintln('\t', "Communication Scope : ", enum_subnet_scope_string(subnet_scope) );
}



/////////////////////////////////////////////////////////////////////

void LoomCommPlat::package(JsonObject json)
{

}

/////////////////////////////////////////////////////////////////////
void LoomCommPlat::convert_string_to_bundle(char* osc_string, OSCBundle& bndl) 
{
	// might just always try to uncompress, as other device may be compressing
	// uncompress_message_string(osc_string);
	original_convert_string_to_bundle(osc_string, bndl);
}

/////////////////////////////////////////////////////////////////////
void LoomCommPlat::convert_bundle_to_string(OSCBundle& bndl, char* osc_string) 
{
	// This is done in case the bundle converts to a string larger than
	// 251 characters before compression
	char larger_buf[384];
	memset(larger_buf, '\0', sizeof(larger_buf));

	original_convert_bundle_to_string(bndl, (char*)larger_buf);

	// if (compress_messages) { 
	// 	compress_message_string((char*)larger_buf);
	// }

	snprintf(osc_string, 251, "%s\0", larger_buf);

	// Remove occasional trailing space
	if (osc_string[strlen(osc_string)-1] == 32 ) {
		osc_string[strlen(osc_string)-1] = '\0';
	}
}

/////////////////////////////////////////////////////////////////////
// --- Private Methods ---
void LoomCommPlat::original_convert_string_to_bundle(char* osc_string, OSCBundle& bndl) 
{
	bndl.empty();
	data_value value_union;
	char buf[strlen(osc_string)+1];
	char *p = buf, *p2 = NULL;
	char *token = NULL, *msg_token = NULL; 
	strcpy(buf, osc_string);
	OSCMessage *msg;
	msg_token = strtok_r(p, " ", &p);

	while ( (msg_token != NULL) & (strlen(msg_token) > 0) ) 
	{
		p2 = msg_token;
		token = strtok_r(p2, ",", &p2);
		msg = &(bndl.add(token));
		token = strtok_r(NULL, ",", &p2);
		int k = 1;
		while (token != NULL & strlen(token) > 0) 
		{
			switch(token[0]) {
				case 'f':
					value_union.u = strtoul(&token[1], NULL, 0);
					msg->add(value_union.f);
					break;
				case 'i':
					value_union.u = strtoul(&token[1], NULL, 0);
					msg->add(value_union.i);
					break;
				case 's': 
					msg->add(&token[1]);
					break;
			}
			token = strtok_r(p2, ",", &p2);

		}
		msg_token = strtok_r(p, " ", &p);
	}
}

/////////////////////////////////////////////////////////////////////
void LoomCommPlat::original_convert_bundle_to_string(OSCBundle& bndl, char* osc_string) 
{
	char data_type;
	data_value value;
	int addr_len = 40;
	OSCMessage* msg; 
	char addr_buf[addr_len];

	memset(osc_string, '\0', sizeof(osc_string));

	for (int i = 0; i < bndl.size(); i++) {
		msg = bndl.getOSCMessage(i);
		memset(addr_buf, '\0', addr_len);
		msg->getAddress(addr_buf, 0);
		strcat(osc_string, addr_buf);

		for (int j = 0; j < msg->size(); j++) {
			data_type = msg->getType(j);
			switch (data_type) {
				case 'f':
					value.f = msg->getFloat(j);
					snprintf(addr_buf, addr_len, ",f%lu", value.u);
					strcat(osc_string, addr_buf);
					break;
				case 'i':
					value.i = msg->getInt(j);
					snprintf(addr_buf, addr_len, ",i%lu", value.u);
					strcat(osc_string, addr_buf);
					break;
				case 's':
					char data_buf[40];
					msg->getString(j, data_buf, sizeof(data_buf));
					snprintf(addr_buf, addr_len, ",s%s", data_buf);
					strcat(osc_string, addr_buf);
					break;
				default:
					if (data_type != '\0') {
						LPrintln("Invalid message arg type");
					}
					break;
			}
		}
		if (msg != NULL) strcat(osc_string, " ");
	}
}
