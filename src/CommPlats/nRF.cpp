///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_nRF.cpp
/// @brief		File for Loom_nRF implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "nRF.h"

///////////////////////////////////////////////////////////////////////////////
Loom_nRF::Loom_nRF(
		LoomManager* manager,
		const uint16_t		max_message_len,
		const uint8_t		address,
		const uint8_t		data_rate,
		const uint8_t		power_level,
		const uint8_t		retry_count,
		const uint16_t		retry_timeout,
		const uint8_t		multicast_level	
	)
	: LoomCommPlat(manager, "nRF", Type::nRF, max_message_len )
	, data_rate(data_rate)
	, power_level(power_level)
	, retry_count(retry_count)
	, retry_timeout(retry_timeout)
	, multicast_level(multicast_level)
	, radio( RF24(5,6) )
{
	// Create nRF manager objects
	// radio	= new RF24(5,6);
	network	= new RF24Network(radio);

	SPI.begin();
	radio.begin();

	// Set data rate
	print_module_label();
	LPrint("Setting nRF data rate to: ");
	switch (data_rate) {
		case 0: 
			LPrintln("Default (1Mbps)");
			break; 
		case 1: 
			radio.setDataRate(RF24_250KBPS);
			LPrintln("250 kbps");
			break;
		case 2: 
			radio.setDataRate(RF24_1MBPS);
			LPrintln("1 Mbps");
			break;
		case 3: 
			radio.setDataRate(RF24_2MBPS);
			LPrintln("2 Mbps");
			break;
	}

	// Set power level
	print_module_label();
	LPrint("Setting nRF power level to: ");
	switch (power_level) {
		case 0: 
			LPrintln("Default");
			break; 
		case 1: 
			radio.setPALevel(RF24_PA_MIN);
			LPrintln("RF24_PA_MIN (-18dBm)");
			break;
		case 2: 
			radio.setPALevel(RF24_PA_LOW);
			LPrintln("RF24_PA_LOW (-12dBm)");
			break;
		case 3: 
			radio.setPALevel(RF24_PA_HIGH);
			LPrintln("RF24_PA_HIGH (-6dBM)");
			break;
		case 4: 
			radio.setPALevel(RF24_PA_MAX);
			LPrintln("RF24_PA_MAX (0dBm)");
			break;
	}

	// Set retry behavior
	// this.retry_count 	= retry_count;
	// this.retry_timeout	= retry_timeout;
	if (  ( (retry_timeout >= 0) && (retry_timeout <= 15) ) &&
		  ( (retry_count >= 0) && (retry_count <= 15) )    ) {

		radio.setRetries(retry_timeout, retry_count);

		print_module_label();
		LPrintln("\tRetry delay: ", 250*retry_timeout, " microseconds");
		LPrintln("\tRetry count: ", retry_count);
	} 

	// Begin network manager with provided address
	network->begin(90, address); // determine good way to convert change to octal address?
}

///////////////////////////////////////////////////////////////////////////////
Loom_nRF::Loom_nRF(LoomManager* manager, JsonArrayConst p)
	: Loom_nRF(manager, EXPAND_ARRAY(p, 7) ) {}

///////////////////////////////////////////////////////////////////////////////
Loom_nRF::~Loom_nRF() 
{
	delete network;
// 	delete radio;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_nRF::add_config(JsonObject json)
{
	// add_config_aux(json, module_name,
	// 	module_name,
	// 	max_message_len, address, data_rate, power_level,
	// 	retry_count, retry_timeout, multicast_level	
	// );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_nRF::print_config() const
{
	LoomCommPlat::print_config();

	LPrintln("\tAddress          : ", address );
	LPrintln("\tData Rate        : ", data_rate );
	LPrintln("\tPower Level      : ", power_level );
	LPrintln("\tRetry Count      : ", retry_count );
	LPrintln("\tRetry Timeout    : ", retry_timeout );
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_nRF::receive_blocking_impl(JsonObject json, uint max_wait_time) 
{
	const unsigned long start_time = millis();
	do {
		network->update();                      // Check the network regularly

		while ( network->available() ) {        // Is there anything ready for us?
			RF24NetworkHeader header;          // If so, grab it and print it out
			char buffer[max_message_len];
			memset(buffer, '\0', max_message_len);
			if (network->read(header, &buffer, max_message_len-1) )
				return msgpack_buffer_to_json(buffer, json);
		}
	} while ( (millis() - start_time) < max_wait_time );
	return false;
}

// ///////////////////////////////////////////////////////////////////////////////
bool Loom_nRF::send_impl(JsonObject json, const uint8_t destination) 
{
	char buffer[max_message_len];
	bool to_msgpack = json_to_msgpack_buffer(json, buffer, max_message_len);
	if (!to_msgpack) return false;

	RF24NetworkHeader header(destination);
	bool is_sent = network->write( header, buffer, measureMsgPack(json) );

	print_module_label();
	LPrintln("Send " , (is_sent) ? "successful" : "failed" );
	return is_sent;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_nRF::broadcast_impl(JsonObject json)
{
	char buffer[max_message_len];
	bool to_msgpack = json_to_msgpack_buffer(json, buffer, max_message_len);
	if (!to_msgpack) {
		print_module_label();
		LPrintln("Failed to convert to msgpack");
		return;
	}

	RF24NetworkHeader header(00);
	network->multicast( header, buffer, measureMsgPack(json), multicast_level );

	print_module_label();
	LPrintln("Broadcasted");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_nRF::set_address(const uint8_t addr)    // Need to test this
{ 
	address = addr;
	network->begin(90, address);
}

///////////////////////////////////////////////////////////////////////////////


