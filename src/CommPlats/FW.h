///////////////////////////////////////////////////////////////////////////////
///
/// @file		FW.h
/// @brief		File for FW implementation.
/// @author		Jared Smith
/// @date		2021
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////
/// NOTE: Make sure to change the value of RH_SERIAL_MAX_PAYLOAD_LEN in RH_Serial.h
/// if you want to send large payloads.  By default, this value is not large enough
/// to use the example sketches.
///////////////////////////////////////////////////////////////////////////////
#ifdef LOOM_INCLUDE_RADIOS
#pragma once

#include "CommPlat.h"

#include <HardwareSerial.h>
#include <RH_Serial.h>
#include <RHReliableDatagram.h>

namespace Loom {

class FW : public CommPlat {
protected:
  // RadioHead objects
  HardwareSerial &serial1;
  RH_Serial driver;
  RHReliableDatagram manager;

  uint8_t address;///< Device Address    (should this be part of LoomCommPlat? – maybe not as each platform handles addresses differently)
  uint8_t max_message_len;
  uint8_t retry_count;///< Number of transmission retries allowed
  uint16_t retry_timeout;///< Delay between transmission retries (in milliseconds)

  bool receive_blocking_impl(JsonObject json, uint max_wait_time) override;

public:
  bool send_impl(JsonObject json, const uint8_t destination);
  FW(
    const uint16_t max_message_len = RH_SERIAL_MAX_MESSAGE_LEN,///If things aren't sending, make sure to check this value in RH_Serial.h
    const uint8_t retry_count = 3,
    const uint16_t retry_timeout = 200
    );
  FW(JsonArrayConst p);

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void add_config(JsonObject json) override;

	void power_up() override;
	void power_down() override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void print_config() const override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	uint8_t get_address() const override { return address; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	void set_address(const uint8_t addr) override;

private:

};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, FW, "FW");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_RADIOS


