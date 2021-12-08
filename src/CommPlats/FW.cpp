///////////////////////////////////////////////////////////////////////////////
///
/// @file		FW.cpp
/// @brief		File for Loom_FW definition.
/// @author		Jared Smith
/// @date		2021
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "FW.h"

//////////////////////////////////////////////////////////////////////////////

Loom_FW::Loom_FW(
		LoomManager* device_manager,
		const uint16_t		max_message_len,
		const uint8_t		address,
		const uint8_t		retry_count,
		const uint16_t		retry_timeout)
  : LoomCommPlat(device_manager, "FW", Type::FW, max_message_len )
  , max_message_len(max_message_len)
  , address(address)
  , retry_count(retry_count)
  , retry_timeout(retry_timeout)
  , serial1(Serial1)///< Adding to option to change the serial port might be good at some point.
  , driver(serial1)
  , manager(driver, address)
{
  serial1.begin(115200);

  LPrintln("\tMax message length:", max_message_len);

  // Set Retry Delay
  print_module_label();
  LPrintln("\tSetting retry timeout to ", retry_timeout, " ms");
  manager.setTimeout(retry_timeout);

  // Set Max Retry Count
  print_module_label();
  LPrintln("\tSetting max retry count ", retry_count);
  manager.setRetries(retry_count);

  bool status = false;
  status = manager.init();
  print_module_label();
  LPrintln("\tInitializing Manager ", (status) ? "Success" : "Failed");
  // driver.sleep();

};

Loom_FW::Loom_FW(LoomManager* device_manager, JsonArrayConst p)
  : Loom_FW(device_manager, EXPAND_ARRAY(p, 4) ) {}

/// Note: the send and receive implementations are almost exactly the same as LoRa
/// since they both use RadioHead.
bool Loom_FW::send_impl(JsonObject json, const uint8_t destination)
{
  char buffer[max_message_len];
  bool to_msgpack = json_to_msgpack_buffer(json, buffer, max_message_len);
  if (!to_msgpack) return false;

  bool is_sent = manager.sendtoWait( (uint8_t*)buffer, measureMsgPack(json), destination );

  print_module_label();
  LPrintln("Send " , (is_sent) ? "successful" : "failed" );
  signal_strength = driver.lastRssi();
  // driver.sleep();
  return is_sent;
}

bool Loom_FW::receive_blocking_impl(JsonObject json, uint max_wait_time)
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

  // driver.sleep();
  return status;
}

void Loom_FW::add_config(JsonObject json)
{ }

///////////////////////////////////////////////////////////////////////////////
void Loom_FW::power_up()
{
	driver.available();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FW::power_down()
{
	driver.sleep();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FW::print_config() const
{
	LoomCommPlat::print_config();

	LPrintln("\tAddress       : ", address );
	LPrintln("\tRetry Count   : ", retry_count );
	LPrintln("\tRetry Timeout : ", retry_timeout );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FW::set_address(const uint8_t addr)
{
	address = addr;
	manager.setThisAddress(addr);
	driver.sleep();
}
