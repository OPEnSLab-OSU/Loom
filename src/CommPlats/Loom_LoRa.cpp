
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
	: LoomCommPlat( module_name, Type::LoRa, max_message_len )
	, address(address)
	, power_level( ( (power_level >= 5) && (power_level <= 23) ) ? power_level : 23 )
	, retry_count(retry_count)
	, retry_timeout(retry_timeout)
	, driver( RH_RF95(RFM95_CS, RFM95_INT) )

{ 
	// this->driver         = new RH_RF95(RFM95_CS, RFM95_INT);
	// this->manager        = new RHReliableDatagram(*driver, address);
	this->manager        = new RHReliableDatagram(driver, address);

	pinMode(8, INPUT_PULLUP);
	pinMode(RFM95_RST, OUTPUT);
	digitalWrite(RFM95_RST, HIGH);

	bool status;

	// Initialize Manager
	status = manager->init();
	print_module_label();
	LPrintln("\tInitializing Manager ", (status) ? "Success" : "Failed");
	
	// Set Frequency
	status = driver.setFrequency(RF95_FREQ);
	print_module_label();
	LPrintln( "\tSetting Frequency ", (status) ? "Success" : "Failed" );

	// Set Power Level
	print_module_label();
	LPrintln("\tSetting Power Level to ", power_level);
	driver.setTxPower(power_level, false);

	// Set Retry Delay
	print_module_label();
	LPrintln("\tSetting retry timeout to ", retry_timeout, " ms");
	manager->setTimeout(retry_timeout);

	// Set Max Retry Count
	print_module_label();
	LPrintln("\tSetting max retry count ", retry_count);
	manager->setRetries(retry_count);

	driver.setModemConfig(RH_RF95::Bw500Cr45Sf128);
}

///////////////////////////////////////////////////////////////////////////////
Loom_LoRa::Loom_LoRa(JsonArrayConst p)
	: Loom_LoRa( EXPAND_ARRAY(p, 6) ) {}

///////////////////////////////////////////////////////////////////////////////
Loom_LoRa::~Loom_LoRa() 
{
	// delete driver;
	delete manager;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LoRa::add_config(JsonObject json)
{
	// add_config_aux(json, module_name,
	// 	module_name, 
	// 	max_message_len, address, power_level, retry_count, retry_timeout
	// );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LoRa::print_config() 
{
	LoomCommPlat::print_config();

	LPrintln("\tAddress       : ", address );
	LPrintln("\tPower Level   : ", power_level );
	LPrintln("\tRetry Count   : ", retry_count );
	LPrintln("\tRetry Timeout : ", retry_timeout );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LoRa::set_address(uint8_t addr)    // Need to test this
{ 
	address = addr;
	delete manager;
	manager = new RHReliableDatagram(driver, address);

	// Initialize Manager
	bool status = manager->init();
	print_module_label();
	LPrintln("\tReinit Manager ", (status) ? "Success" : "Failed");
	
	// Set Frequency
	status = driver.setFrequency(RF95_FREQ);
	print_module_label();
	LPrintln( "\tSet Frequency ", (status) ? "Success" : "Failed" );

	// Set Retry Delay
	print_module_label();
	LPrintln("\tSet retry timeout to ", retry_timeout, " ms");
	manager->setTimeout(retry_timeout);

	// Set Max Retry Count
	print_module_label();
	LPrintln("\tSet max retry count ", retry_count);
	manager->setRetries(retry_count);
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
			signal_strength = driver.lastRssi();
			bool status = msgpack_buffer_to_json(buffer, json);
			return status;
		}
	} 

	return false;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_LoRa::send(JsonObject json, uint8_t destination) 
{
	char buffer[max_message_len];
	bool to_msgpack = json_to_msgpack_buffer(json, buffer, max_message_len);
	if (!to_msgpack) return false;

	bool is_sent = manager->sendtoWait( (uint8_t*)buffer, measureMsgPack(json), destination );

	print_module_label();
	LPrintln("Send " , (is_sent) ? "successful" : "failed" );
	signal_strength = driver.lastRssi(); 
	return is_sent;
}

///////////////////////////////////////////////////////////////////////////////

