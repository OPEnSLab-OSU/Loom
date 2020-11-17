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
		const uint16_t		retry_timeout,
		const bool			override_name
	)
	: LoomCommPlat(device_manager, "LoRa", Type::LoRa, max_message_len )
	, address(address)
	, power_level( ( (power_level >= 5) && (power_level <= 23) ) ? power_level : 23 )
	, retry_count(retry_count)
	, retry_timeout(retry_timeout)
	, driver{ RFM95_CS, RFM95_INT }
	, manager{ driver, address }
{
  LMark;
	pinMode(8, INPUT_PULLUP);
  LMark;
	pinMode(RFM95_RST, OUTPUT);
  LMark;
	digitalWrite(RFM95_RST, HIGH);
  LMark;

	bool status;
  LMark;

	// Initialize Manager
	status = manager.init();
  LMark;
	print_module_label();
  LMark;
	LPrintln("\tInitializing Manager ", (status) ? "Success" : "Failed");
  LMark;

	// Set Frequency
	status = driver.setFrequency(RF95_FREQ);
  LMark;
	print_module_label();
  LMark;
	LPrintln( "\tSetting Frequency ", (status) ? "Success" : "Failed" );
  LMark;

	// Set Power Level
	print_module_label();
  LMark;
	LPrintln("\tSetting Power Level to ", power_level);
  LMark;
	driver.setTxPower(power_level, false);
  LMark;

	// Set Retry Delay
	print_module_label();
  LMark;
	LPrintln("\tSetting retry timeout to ", retry_timeout, " ms");
  LMark;
	manager.setTimeout(retry_timeout);
  LMark;

	// Set Max Retry Count
	print_module_label();
  LMark;
	LPrintln("\tSetting max retry count ", retry_count);
  LMark;
	manager.setRetries(retry_count);
  LMark;

	driver.setModemConfig(RH_RF95::Bw500Cr45Sf128);
  LMark;

	driver.sleep();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_LoRa::Loom_LoRa(LoomManager* device_manager, JsonArrayConst p)
	: Loom_LoRa(device_manager, EXPAND_ARRAY(p, 6) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_LoRa::add_config(JsonObject json)
{
  LMark;
	// add_config_aux(json, module_name,
	// 	module_name,
	// 	max_message_len, address, power_level, retry_count, retry_timeout
	// );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LoRa::power_up() {
  LMark;
	driver.available();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LoRa::power_down() {
  LMark;
	driver.sleep();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LoRa::print_config() const
{
  LMark;
	LoomCommPlat::print_config();
  LMark;

	LPrintln("\tAddress       : ", address );
  LMark;
	LPrintln("\tPower Level   : ", power_level );
  LMark;
	LPrintln("\tRetry Count   : ", retry_count );
  LMark;
	LPrintln("\tRetry Timeout : ", retry_timeout );
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_LoRa::set_address(const uint8_t addr)    // Need to test this
{
  LMark;
	address = addr;
  LMark;
	manager.setThisAddress(addr);
  LMark;
	driver.sleep();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_LoRa::receive_blocking_impl(JsonObject json, uint max_wait_time)
{
  LMark;
	bool status = false;
  LMark;
	uint8_t len = max_message_len;
  LMark;
	uint8_t from;
  LMark;
	char buffer[max_message_len];
  LMark;
	memset(buffer, '\0', max_message_len);
  LMark;
	if (max_wait_time == 0)
		status = manager.recvfromAck( (uint8_t*)buffer, &len, &from );
	else
		status = manager.recvfromAckTimeout( (uint8_t*)buffer, &len, max_wait_time, &from );
  LMark;

	if (status) {
   	LMark;
		signal_strength = driver.lastRssi();
   	LMark;
		status = msgpack_buffer_to_json(buffer, json);
  	LMark;
	}
	
	print_module_label();
	LPrintln("Receive ", (status) ? "successful" : "failed");

	driver.sleep();
  LMark;
	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_LoRa::send_impl(JsonObject json, const uint8_t destination)
{
  LMark;
	char buffer[max_message_len];
  LMark;
	bool to_msgpack = json_to_msgpack_buffer(json, buffer, max_message_len);
  LMark;
	if (!to_msgpack) return false;
  LMark;

	bool is_sent = manager.sendtoWait( (uint8_t*)buffer, measureMsgPack(json), destination );
  LMark;

	print_module_label();
  LMark;
	LPrintln("Send " , (is_sent) ? "successful" : "failed" );
  LMark;
	signal_strength = driver.lastRssi();
  LMark;
	driver.sleep();
  LMark;
	return is_sent;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_LoRa::send_raw(uint8_t* bytes, const uint8_t len, const uint8_t destination) {
  LMark;
	bool is_sent = manager.sendtoWait(bytes, len, destination);
  LMark;

	print_module_label();
  LMark;
	LPrintln("Send " , (is_sent) ? "successful" : "failed" );
  LMark;
	signal_strength = driver.lastRssi();
  LMark;
	driver.sleep();
  LMark;
	return is_sent;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_LoRa::receive_blocking_raw(uint8_t* dest, const uint8_t maxlen, const uint max_wait_time) {
  LMark;
	bool status;
  LMark;
	uint8_t len = maxlen;
  LMark;
	uint8_t from;
  LMark;
	memset(dest, '\0', maxlen);
  LMark;
	if (max_wait_time == 0)
		status = manager.recvfromAck( dest, &len, &from );
	else
		status = manager.recvfromAckTimeout( dest, &len, max_wait_time, &from );
  LMark;

	if (status)
		signal_strength = driver.lastRssi();
  LMark;

	driver.sleep();
  LMark;
	return status;
}
