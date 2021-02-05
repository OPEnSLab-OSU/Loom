///////////////////////////////////////////////////////////////////////////////
///
/// @file		nRF.cpp
/// @brief		File for nRF implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_RADIOS

#include "nRF.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
nRF::nRF(
		const uint16_t		max_message_len,
		const uint8_t		address,
		const uint8_t		data_rate,
		const uint8_t		power_level,
		const uint8_t		retry_count,
		const uint16_t		retry_timeout,
		const uint8_t		multicast_level,
		const bool			override_name
	)
	: CommPlat("nRF", max_message_len )
	, data_rate(data_rate)
	, power_level(power_level)
	, retry_count(retry_count)
	, retry_timeout(retry_timeout)
	, multicast_level(multicast_level)
	, radio( RF24(5,6) )
{
  LMark;
	// Create nRF manager objects
	// radio	= new RF24(5,6);
	network	= new RF24Network(radio);
  LMark;

	SPI.begin();
  LMark;
	radio.begin();
  LMark;

	// Set data rate
	print_module_label();
  LMark;
	LPrint("Setting nRF data rate to: ");
  LMark;
	switch (data_rate) {
		case 0:
    	LMark;
			LPrintln("Default (1Mbps)");
    	LMark;
			break;
		case 1:
    	LMark;
			radio.setDataRate(RF24_250KBPS);
    	LMark;
			LPrintln("250 kbps");
    	LMark;
			break;
		case 2:
    	LMark;
			radio.setDataRate(RF24_1MBPS);
    	LMark;
			LPrintln("1 Mbps");
    	LMark;
			break;
		case 3:
    	LMark;
			radio.setDataRate(RF24_2MBPS);
    	LMark;
			LPrintln("2 Mbps");
    	LMark;
			break;
	}

	// Set power level
	print_module_label();
  LMark;
	LPrint("Setting nRF power level to: ");
  LMark;
	switch (power_level) {
		case 0:
    	LMark;
			LPrintln("Default");
    	LMark;
			break;
		case 1:
    	LMark;
			radio.setPALevel(RF24_PA_MIN);
    	LMark;
			LPrintln("RF24_PA_MIN (-18dBm)");
    	LMark;
			break;
		case 2:
    	LMark;
			radio.setPALevel(RF24_PA_LOW);
    	LMark;
			LPrintln("RF24_PA_LOW (-12dBm)");
    	LMark;
			break;
		case 3:
	    LMark;
			radio.setPALevel(RF24_PA_HIGH);
    	LMark;
			LPrintln("RF24_PA_HIGH (-6dBM)");
    	LMark;
			break;
		case 4:
    	LMark;
			radio.setPALevel(RF24_PA_MAX);
    	LMark;
			LPrintln("RF24_PA_MAX (0dBm)");
    	LMark;
			break;
	}

	// Set retry behavior
	// this.retry_count 	= retry_count;
	// this.retry_timeout	= retry_timeout;
	if (  ( (retry_timeout >= 0) && (retry_timeout <= 15) ) &&
		  ( (retry_count >= 0) && (retry_count <= 15) )    ) {
  	LMark;

		radio.setRetries(retry_timeout, retry_count);
  	LMark;

		print_module_label();
   	LMark;
		LPrintln("\tRetry delay: ", 250*retry_timeout, " microseconds");
   	LMark;
		LPrintln("\tRetry count: ", retry_count);
  	LMark;
	}

	// Begin network manager with provided address
	network->begin(90, address); // determine good way to convert change to octal address?
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
nRF::nRF(JsonArrayConst p)
	: nRF(EXPAND_ARRAY(p, 8) ) {}

///////////////////////////////////////////////////////////////////////////////
nRF::~nRF()
{
  LMark;
	delete network;
 	LMark;
// 	delete radio;
}

///////////////////////////////////////////////////////////////////////////////
void nRF::print_config() const
{
  LMark;
	CommPlat::print_config();
  LMark;

	LPrintln("\tAddress          : ", address );
  LMark;
	LPrintln("\tData Rate        : ", data_rate );
  LMark;
	LPrintln("\tPower Level      : ", power_level );
  LMark;
	LPrintln("\tRetry Count      : ", retry_count );
  LMark;
	LPrintln("\tRetry Timeout    : ", retry_timeout );
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
bool nRF::receive_blocking_impl(JsonObject json, uint max_wait_time)
{
	bool status = false;
	LMark;
	const unsigned long start_time = millis();
  LMark;
	do {
   	LMark;
		network->update();                      // Check the network regularly
  	LMark;

		while ( network->available() ) {        // Is there anything ready for us?
    	LMark;
			RF24NetworkHeader header;          // If so, grab it and print it out
    	LMark;
			char buffer[max_message_len];
    	LMark;
			memset(buffer, '\0', max_message_len);
    	LMark;
			if (network->read(header, &buffer, max_message_len-1) )
				LMark;
				status = msgpack_buffer_to_json(buffer, json);
		}
	} while ( (millis() - start_time) < max_wait_time );

	print_module_label();
	LPrintln("Receive ", (status) ? "successful" : "failed");
	return status;
}

// ///////////////////////////////////////////////////////////////////////////////
bool nRF::send_impl(JsonObject json, const uint8_t destination)
{
  LMark;
	char buffer[max_message_len];
  LMark;
	bool to_msgpack = json_to_msgpack_buffer(json, buffer, max_message_len);
  LMark;
	if (!to_msgpack) return false;
  LMark;

	RF24NetworkHeader header(destination);
  LMark;
	bool is_sent = network->write( header, buffer, measureMsgPack(json) );
  LMark;

	print_module_label();
  LMark;
	LPrintln("Send " , (is_sent) ? "successful" : "failed" );
  LMark;
	return is_sent;
}

///////////////////////////////////////////////////////////////////////////////
void nRF::broadcast_impl(JsonObject json)
{
  LMark;
	char buffer[max_message_len];
  LMark;
	bool to_msgpack = json_to_msgpack_buffer(json, buffer, max_message_len);
  LMark;
	if (!to_msgpack) {
   	LMark;
		print_module_label();
   	LMark;
		LPrintln("Failed to convert to msgpack");
   	LMark;
		return;
	}

	RF24NetworkHeader header(00);
  LMark;
	network->multicast( header, buffer, measureMsgPack(json), multicast_level );
  LMark;

	print_module_label();
  LMark;
	LPrintln("Broadcasted");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void nRF::set_address(const uint8_t addr)    // Need to test this
{
  LMark;
	address = addr;
  LMark;
	network->begin(90, address);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_RADIOS
