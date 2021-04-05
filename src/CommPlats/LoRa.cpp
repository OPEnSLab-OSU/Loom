///////////////////////////////////////////////////////////////////////////////
///
/// @file		LoRa.cpp
/// @brief		File for LoRa implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_RADIOS

#include "LoRa.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
LoRa::LoRa(
		const uint16_t		max_message_len,
		const uint8_t		address,
		const uint8_t		power_level,
		const uint8_t		retry_count,
		const uint16_t		retry_timeout,
		const bool			override_name
	)
	: CommPlat("LoRa", max_message_len )
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
  LMark;
	driver.setTxPower(power_level, false);

	// Set Retry Delay
	print_module_label();
	LPrintln("\tSetting retry timeout to ", retry_timeout, " ms");
  LMark;
	manager.setTimeout(retry_timeout);

	// Set Max Retry Count
	print_module_label();
	LPrintln("\tSetting max retry count ", retry_count);
	manager.setRetries(retry_count);
  LMark;

	driver.setModemConfig(RH_RF95::Bw500Cr45Sf128);

	driver.sleep();
}

///////////////////////////////////////////////////////////////////////////////
LoRa::LoRa(JsonArrayConst p)
	: LoRa(EXPAND_ARRAY(p, 6) ) {}

///////////////////////////////////////////////////////////////////////////////
void LoRa::power_up() {
	driver.available();
}

///////////////////////////////////////////////////////////////////////////////
void LoRa::power_down() {
	driver.sleep();
}

///////////////////////////////////////////////////////////////////////////////
void LoRa::print_config() const
{
	CommPlat::print_config();

	LPrintln("\tAddress       : ", address );
	LPrintln("\tPower Level   : ", power_level );
	LPrintln("\tRetry Count   : ", retry_count );
	LPrintln("\tRetry Timeout : ", retry_timeout );
}

///////////////////////////////////////////////////////////////////////////////
void LoRa::set_address(const uint8_t addr)    // Need to test this
{
	address = addr;
  LMark;
	manager.setThisAddress(addr);
	driver.sleep();
}

///////////////////////////////////////////////////////////////////////////////
bool LoRa::receive_blocking_impl(JsonObject json, uint max_wait_time)
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
  LMark;

	if (status) {
   	LMark;
		signal_strength = driver.lastRssi();
   	LMark;
		status = msgpack_buffer_to_json(buffer, json);
	}

	print_module_label();
	LPrintln("Receive ", (status) ? "successful" : "failed");

	driver.sleep();
	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool LoRa::send_impl(JsonObject json, const uint8_t destination)
{
	char buffer[max_message_len];
  LMark;
	bool to_msgpack = json_to_msgpack_buffer(json, buffer, max_message_len);
	if (!to_msgpack) return false;

	bool is_sent = manager.sendtoWait( (uint8_t*)buffer, measureMsgPack(json), destination );

	print_module_label();
	LPrintln("Send " , (is_sent) ? "successful" : "failed" );
  LMark;
	signal_strength = driver.lastRssi();
  LMark;
	driver.sleep();
	return is_sent;
}

///////////////////////////////////////////////////////////////////////////////
bool LoRa::send_raw(uint8_t* bytes, const uint8_t len, const uint8_t destination) {
  LMark;
	bool is_sent = manager.sendtoWait(bytes, len, destination);

	print_module_label();
	LPrintln("Send " , (is_sent) ? "successful" : "failed" );
  LMark;
	signal_strength = driver.lastRssi();
  LMark;
	driver.sleep();
	return is_sent;
}

///////////////////////////////////////////////////////////////////////////////
bool LoRa::receive_blocking_raw(uint8_t* dest, const uint8_t maxlen, const uint max_wait_time) {
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

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_RADIOS
