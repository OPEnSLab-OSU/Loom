
#include "Loom_LoRa.h"


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_LoRa::Loom_LoRa( 	
		const char*		module_name,
		uint16_t		max_message_len,

		uint8_t			address,
		uint8_t			friend_address,
		uint8_t			power_level,
		uint8_t			retry_count,
		uint16_t		retry_timeout 	
	)
	: LoomCommPlat( module_name, max_message_len )

{ 
	this->module_type = ModuleType::LoRa;

	// Create instances of driver and manager
	this->driver         = new RH_RF95(RFM95_CS, RFM95_INT);
	this->manager        = new RHReliableDatagram(*driver, address);
	this->address        = address;
	this->friend_address = friend_address;
	this->power_level    = ( (power_level >= 5) && (power_level <= 23) ) ? power_level : 23;
	this->retry_count    = retry_count;
	this->retry_timeout  = retry_timeout;

	// #if hub_node_type == 0 // if hub
	pinMode(8, INPUT_PULLUP);
	// #endif

	bool status;

	// Initialize Manager
	status = manager->init();
	print_module_label();
	LPrintln("\tInitializing Manager ", (status) ? "Success" : "Failed");
	
	// Set Frequency
	status = driver->setFrequency(RF95_FREQ);
	print_module_label();
	LPrintln( "\tSetting Frequency ", (status) ? "Success" : "Failed" );

	// Set Power Level
	print_module_label();
	LPrintln("\tSetting Power Level to ", this->power_level);
	driver->setTxPower(this->power_level, false);

	// Set Retry Delay
	print_module_label();
	LPrintln("\tSetting retry timeout to ", this->retry_timeout, " ms");
	manager->setTimeout(this->retry_timeout);

	// Set Max Retry Count
	print_module_label();
	LPrintln("\tSetting max retry count ", this->retry_count);
	manager->setRetries(this->retry_count);
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_LoRa::Loom_LoRa(JsonVariant p)
	// : Loom_LoRa(p[0], p[1], p[2], p[3], p[4], p[5], p[6])
	: Loom_LoRa( ARR_EXPAND(p, 7) )
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_LoRa::~Loom_LoRa() 
{
	delete driver;
	delete manager;
}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void Loom_LoRa::print_config() 
{
	LoomCommPlat::print_config();

	LPrintln('\t', "Address             : ", address );
	LPrintln('\t', "Power Level         : ", power_level );
	LPrintln('\t', "Retry Count         : ", retry_count );
	LPrintln('\t', "Retry Timeout       : ", retry_timeout );
}

/////////////////////////////////////////////////////////////////////
bool Loom_LoRa::receive_bundle(OSCBundle& bndl) 
{
	if ( manager->available() ) {
		uint8_t len = RH_RF95_MAX_MESSAGE_LEN;
		uint8_t from;
		uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
		memset(buf, '\0', RH_RF95_MAX_MESSAGE_LEN);

		if ( manager->recvfromAck(buf, &len, &from) ) {

			signal_strength = driver->lastRssi();

			if (print_verbosity == Verbosity::V_HIGH) {
				LPrintln("Received: ", (const char*)buf);
				LPrintln("Len: ", len);
			}

			// This is done just in case the compressed string, uncompresses to more than 251 characters
			char larger_buf[384];
			memset(larger_buf, '\0', sizeof(larger_buf));
			strcpy(larger_buf, (const char*)buf);

			if (print_verbosity == Verbosity::V_HIGH) {
				LPrintln("Memset larger_buf");
			}

			LoomCommPlat::convert_string_to_bundle( (char*)larger_buf, bndl ); 

			if (print_verbosity == Verbosity::V_HIGH) {
				LPrintln("Converted string to bundle");
			}

			return true;
			 
		} // of if (manager.recvfromAck(buf, &len, &from))
	} // of if (manager.available()) 

	return false;
}

/////////////////////////////////////////////////////////////////////
bool Loom_LoRa::send_bundle(OSCBundle& bndl, uint16_t destination) 
// bool send_bundle(OSCBundle& bndl, uint16_t destination=01) 
// bool send_bundle(OSCBundle& bndl) 
{
	LPrintln("Sending LoRa bundle to address: ", destination);

	char message[RH_RF95_MAX_MESSAGE_LEN];
	memset(message, '\0', sizeof(message));
	LoomCommPlat::convert_bundle_to_string(bndl, message);

	if (print_verbosity == Verbosity::V_HIGH) {
		LPrintln(message);
		LPrintln("Message length: ", strlen(message));
	}

	// bool is_sent = manager->sendtoWait( (uint8_t*)message, strlen(message)+1, destination );
	bool is_sent = manager->sendtoWait( (uint8_t*)message, strlen(message), destination );

	LPrintln("Send LoRa bundle " , (is_sent) ? "successful" : "failed" );

	signal_strength = driver->lastRssi(); 

	return is_sent;
}

/////////////////////////////////////////////////////////////////////
bool Loom_LoRa::send_bundle(OSCBundle& bndl) 
{
	send_bundle(bndl, friend_address);
}

/////////////////////////////////////////////////////////////////////
void Loom_LoRa::broadcast_bundle(OSCBundle& bndl) 
{

}

/////////////////////////////////////////////////////////////////////
void Loom_LoRa::set_address(uint addr)    // Need to test this
{ 
	address = addr;
	delete manager;
	manager = new RHReliableDatagram(*driver, address);

	// Initialize Manager
	bool status = manager->init();
	print_module_label();
	LPrintln("\tReinitializing Manager ", (status) ? "Success" : "Failed");
	
	// Set Frequency
	status = driver->setFrequency(RF95_FREQ);
	print_module_label();
	LPrintln( "\tSetting Frequency ", (status) ? "Success" : "Failed" );

	// Set Retry Delay
	print_module_label();
	LPrintln("\tSetting retry timeout to ", retry_timeout, " ms");
	manager->setTimeout(retry_timeout);

	// Set Max Retry Count
	print_module_label();
	LPrintln("\tSetting max retry count ", retry_count);
	manager->setRetries(retry_count);
}

/////////////////////////////////////////////////////////////////////
uint Loom_LoRa::get_address() 
{ 
	return address; 
}

/////////////////////////////////////////////////////////////////////
void Loom_LoRa::set_friend_address(uint addr) 
{ 
	friend_address = addr; 
}

/////////////////////////////////////////////////////////////////////
uint Loom_LoRa::get_friend_address() 
{ 
	return friend_address; 
}



/////////////////////////////////////////////////////////////////////
bool		receive_json(JsonObject json) 
{

}

/////////////////////////////////////////////////////////////////////
bool		send_json(JsonObject json, uint16_t destination) 
{

}

/////////////////////////////////////////////////////////////////////
bool		send_json(JsonObject json) 
{

}









