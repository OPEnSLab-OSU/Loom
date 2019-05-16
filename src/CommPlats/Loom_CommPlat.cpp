
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


// char* LoomCommPlat::enum_subnet_scope_string(CommScope s)
// {
// 	switch(s) {
// 		case SCOPE_DEVICE      : return "Device";
// 		case SCOPE_SUBNET      : return "Subnet";
// 		case SCOPE_FAMILY      : return "Family";
// 		case SCOPE_SUBNET_ONLY : return "Subnet Only";
// 		case SCOPE_FAMILY_ONLY : return "Family Only";
// 		case SCOPE_ANY         : return "Any";
// 	}
// }


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
LoomCommPlat::LoomCommPlat( 
		const char*		module_name, 
		uint			max_message_len, 
		bool			compress_messages 
	) 
	: LoomModule( module_name )
{
	// LPrintln("LoomCommPlat Constructor 1");
	this->max_message_len   = max_message_len;
	// this->subnet_scope      = subnet_scope; 
	this->compress_messages = compress_messages;
	this->signal_strength   = 0;
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
	LPrintln('\t', "Compress Messages   : ", (compress_messages) ? "Enabled" : "Disabled" );
}

/////////////////////////////////////////////////////////////////////
void LoomCommPlat::package(OSCBundle& bndl, char* suffix) 
{

}

/////////////////////////////////////////////////////////////////////

void LoomCommPlat::package(JsonObject json)
{

}

/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
void LoomCommPlat::set_compress_messages(bool c) 
{ 
	compress_messages = c; 
}

/////////////////////////////////////////////////////////////////////
bool LoomCommPlat::get_compress_messages() 
{ 
	return compress_messages; 
}

/////////////////////////////////////////////////////////////////////
	// returns false if filtered out

// might not actually empty bundle but just return a boolean
	// depends on whether filtering is done at routing or receiving

// need to actually check names
	// unless that is done via routing?

// maybe return offset as well?


// Offset index where message command starts (i.e. after addressing) for message routing later
bool LoomCommPlat::scope_filter(OSCBundle& bndl, int* offset) 
{
	// CommPlat needs to be associated with a LoomDevice to filter 
	if (device_manager == NULL) return true; 

	bool pass = false;
	char type = get_message_type( bndl.getOSCMessage(0) );

	char address[50], test[50];

	bndl.getOSCMessage(0)->getAddress(address, 2); // offset of 2 to skip '/<type>'

	switch (type) {
		case 'D': device_manager->packet_header_device(test); break;
		case 'S': device_manager->packet_header_subnet(test); break;
		case 'F': device_manager->packet_header_family(test); break;
		default: break;
	}

	// Remove type character
	strcpy(test, test+2);

	// if ( strlen(test) == 0 ) {
	// 	LPrintln("Out of scope");
	// 	bndl.empty();
	// 	return false; 
	// }

	// LPrintln("[Scope Filter] Address: ", address);
	// LPrintln("[Scope Filter] Test: ", test);
	// LPrintln("[Scope Filter] len(address): ", strlen(address) );
	// LPrintln("[Scope Filter] len(test): ", strlen(test) );


	// Compare addresses
	// if ( strncmp(address, test+2, strlen(test)-2 ) == 0 ) {
	if ( strncmp(address, test, strlen(test) ) == 0 ) {
		pass = true;
	}

	LPrintln("[Scope Filter] Pass: ", (pass) ? "true" : "false" );

	// Empty bundle if out of scope
	if (!pass) bndl.empty();


	if (offset) {
		*offset = (pass) ? strlen(test)+2 : -1;

	// 	if (pass) {

	// 		// fix offset to return correct number - check type to determine what number '/' to search for 

	// // const char* nth_strchr(const char* s, char c, int n)
	// 		int section = 0;
	// 		switch (type) {
	// 			case 'D': section = 6; break;
	// 			case 'S': section = 4; break;
	// 			case 'F': section = 3; break;
	// 			default: break;
	// 		}
	// 		*offset = nth_strchr(address, '/', section) - address;
		
	// 	} else {
	// 		*offset = -1;
	// 	}

	}

	return pass;
}

/////////////////////////////////////////////////////////////////////
bool LoomCommPlat::scope_filter(OSCBundle& bndl) 
{
	return scope_filter(bndl, NULL);
}

/////////////////////////////////////////////////////////////////////
void LoomCommPlat::convert_string_to_bundle(char* osc_string, OSCBundle& bndl) 
{
	// might just always try to uncompress, as other device may be compressing
	uncompress_message_string(osc_string);
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

	if (compress_messages) { 
		compress_message_string((char*)larger_buf);
	}

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

/////////////////////////////////////////////////////////////////////
void LoomCommPlat::compress_message_string(char* osc_string) 
{
	const char* cPtr = nth_strchr(osc_string, '/', 3);

	// Only try to compress if possible 
	int third_slash = cPtr - (char*)osc_string;
	if ( (third_slash > 0) && (third_slash < 30) ) {
		
		char buf[30];
		snprintf(buf, cPtr-osc_string+2, "%s\0", osc_string); // Copy compressable header to buf
		str_replace((char*)cPtr, buf, "%");
	}
}

/////////////////////////////////////////////////////////////////////
void LoomCommPlat::uncompress_message_string(char* osc_string) 
{
	// Only try to uncompress if it was compressed
	if ( strstr(osc_string, "%") ) {
		char buf[30];
		const char* cPtr = nth_strchr( osc_string, '/', 3 );
		snprintf( buf, cPtr-osc_string+2, "%s", osc_string );
		str_replace( (char*)cPtr, "%", buf );		
	}
}
