
#include "Loom_LoRa.h"


///////////////////////////////////////////////////////////////////////////////
Loom_LoRa::Loom_LoRa( 	
		const char*		module_name,
		uint16_t		max_message_len,

		uint8_t			address,
		uint8_t			power_level,
		uint8_t			retry_count,
		uint16_t		retry_timeout 	
	)
	: LoomCommPlat( module_name, max_message_len )

{ 
	this->module_type = LoomModule::Type::LoRa;

	// Create instances of driver and manager
	this->driver         = new RH_RF95(RFM95_CS, RFM95_INT);
	this->manager        = new RHReliableDatagram(*driver, address);
	this->address        = address;
	this->power_level    = ( (power_level >= 5) && (power_level <= 23) ) ? power_level : 23;
	this->retry_count    = retry_count;
	this->retry_timeout  = retry_timeout;

	pinMode(8, INPUT_PULLUP);

	pinMode(RFM95_RST, OUTPUT);
	digitalWrite(RFM95_RST, HIGH);

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

	driver->setModemConfig(RH_RF95::Bw500Cr45Sf128);
}

///////////////////////////////////////////////////////////////////////////////
Loom_LoRa::Loom_LoRa(JsonArrayConst p)
	: Loom_LoRa( EXPAND_ARRAY(p, 6) ) {}

///////////////////////////////////////////////////////////////////////////////
Loom_LoRa::~Loom_LoRa() 
{
	delete driver;
	delete manager;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LoRa::print_config() 
{
	LoomCommPlat::print_config();

	LPrintln('\t', "Address             : ", address );
	LPrintln('\t', "Power Level         : ", power_level );
	LPrintln('\t', "Retry Count         : ", retry_count );
	LPrintln('\t', "Retry Timeout       : ", retry_timeout );
}

///////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
uint Loom_LoRa::get_address() 
{ 
	return address; 
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_LoRa::receive(JsonObject json) 
{
	if ( manager->available() ) {
		uint8_t len = max_message_len;
		uint8_t from;
		char buffer[max_message_len];
		memset(buffer, '\0', max_message_len);

		if ( manager->recvfromAck( (uint8_t*)buffer, &len, &from) ) {
			signal_strength = driver->lastRssi();
			bool status = msgpack_buffer_to_json(buffer, json);
			return status;
		}
	} 

	return false;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_LoRa::send(JsonObject json, uint16_t destination) 
{
	char buffer[max_message_len];
	bool to_msgpack = json_to_msgpack_buffer(json, buffer, max_message_len);
	if (!to_msgpack) return false;

	bool is_sent = manager->sendtoWait( (uint8_t*)buffer, measureMsgPack(json), destination );

	print_module_label();
	LPrintln("Send " , (is_sent) ? "successful" : "failed" );
	signal_strength = driver->lastRssi(); 
	return is_sent;
}

///////////////////////////////////////////////////////////////////////////////

