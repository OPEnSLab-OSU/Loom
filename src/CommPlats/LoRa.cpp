///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_LoRa.cpp
/// @brief		File for Loom_LoRa implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "LoRa.h"

///////////////////////////////////////////////////////////////////////////////
Loom_LoRa::Loom_LoRa(
		LoomManager* device_manager,
		const uint16_t		max_message_len,
		const uint8_t		address,
		const uint8_t		power_level,
		const uint8_t		retry_count,
		const uint16_t		retry_timeout 	
	)
	: LoomCommPlat(device_manager, "LoRa", Type::LoRa, max_message_len )
	, address(address)
	, power_level( ( (power_level >= 5) && (power_level <= 23) ) ? power_level : 23 )
	, retry_count(retry_count)
	, retry_timeout(retry_timeout)
	, driver{ RFM95_CS, RFM95_INT }
	, manager{ driver, address }
{ 
	pinMode(8, INPUT_PULLUP);
	pinMode(RFM95_RST, OUTPUT);
	digitalWrite(RFM95_RST, HIGH);

	bool status;

	// Initialize Manager
	status = manager.init();
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
	manager.setTimeout(retry_timeout);

	// Set Max Retry Count
	print_module_label();
	LPrintln("\tSetting max retry count ", retry_count);
	manager.setRetries(retry_count);

	driver.setModemConfig(RH_RF95::Bw500Cr45Sf128);

	driver.sleep();
}

///////////////////////////////////////////////////////////////////////////////
Loom_LoRa::Loom_LoRa(LoomManager* device_manager, JsonArrayConst p)
	: Loom_LoRa(device_manager, EXPAND_ARRAY(p, 5) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_LoRa::add_config(JsonObject json)
{
	// add_config_aux(json, module_name,
	// 	module_name, 
	// 	max_message_len, address, power_level, retry_count, retry_timeout
	// );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LoRa::power_up() {
	driver.available();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LoRa::power_down() {
	driver.sleep();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LoRa::print_config() const
{
	LoomCommPlat::print_config();

	LPrintln("\tAddress       : ", address );
	LPrintln("\tPower Level   : ", power_level );
	LPrintln("\tRetry Count   : ", retry_count );
	LPrintln("\tRetry Timeout : ", retry_timeout );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LoRa::set_address(const uint8_t addr)    // Need to test this
{ 
	address = addr;
	manager.setThisAddress(addr);
	driver.sleep();
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_LoRa::receive_blocking_impl(JsonObject json, uint max_wait_time) 
{
	bool status = false;
	uint8_t len = max_message_len;
	uint8_t from;
	char buffer[max_message_len];
	memset(buffer, '\0', max_message_len);
	if (max_wait_time == 0)
		status = manager.recvfromAck( (uint8_t*)buffer, &len, &from );
	else 
		status = manager.recvfromAckTimeout( (uint8_t*)buffer, &len, max_wait_time, &from );
	
	if (status) {
		signal_strength = driver.lastRssi();
		status = msgpack_buffer_to_json(buffer, json);
	}
	
	print_module_label();
	LPrintln("Receive ", (status) ? "successful" : "failed");

	driver.sleep();
	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_LoRa::send_impl(JsonObject json, const uint8_t destination) 
{
	char buffer[max_message_len];
	bool to_msgpack = json_to_msgpack_buffer(json, buffer, max_message_len);
	if (!to_msgpack) return false;

	bool is_sent = manager.sendtoWait( (uint8_t*)buffer, measureMsgPack(json), destination );

	print_module_label();
	LPrintln("Send " , (is_sent) ? "successful" : "failed" );
	signal_strength = driver.lastRssi();
	driver.sleep();
	return is_sent;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_LoRa::send_raw(uint8_t* bytes, const uint8_t len, const uint8_t destination) {
	bool is_sent = manager.sendtoWait(bytes, len, destination);

	print_module_label();
	LPrintln("Send " , (is_sent) ? "successful" : "failed" );
	signal_strength = driver.lastRssi();
	driver.sleep();
	return is_sent;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_LoRa::receive_blocking_raw(uint8_t* dest, const uint8_t maxlen, const uint max_wait_time) {
	bool status;
	uint8_t len = maxlen;
	uint8_t from;
	memset(dest, '\0', maxlen);
	if (max_wait_time == 0)
		status = manager.recvfromAck( dest, &len, &from );
	else 
		status = manager.recvfromAckTimeout( dest, &len, max_wait_time, &from );
	
	if (status)
		signal_strength = driver.lastRssi();

	driver.sleep();
	return status;
}