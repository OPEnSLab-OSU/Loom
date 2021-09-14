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

	SPI.begin();
  LMark;
	radio.begin();

	// Set data rate
	print_module_label();
	LPrint("Setting nRF data rate to: ");
  LMark;
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
    	LMark;
			radio.setPALevel(RF24_PA_MIN);
			LPrintln("RF24_PA_MIN (-18dBm)");
			break;
		case 2:
    	LMark;
			radio.setPALevel(RF24_PA_LOW);
			LPrintln("RF24_PA_LOW (-12dBm)");
			break;
		case 3:
	    LMark;
			radio.setPALevel(RF24_PA_HIGH);
			LPrintln("RF24_PA_HIGH (-6dBM)");
			break;
		case 4:
    	LMark;
			radio.setPALevel(RF24_PA_MAX);
			LPrintln("RF24_PA_MAX (0dBm)");
			break;
	}

	// Set retry behavior
	// this.retry_count 	= retry_count;
	// this.retry_timeout	= retry_timeout;
	if (  ( (retry_timeout >= 0) && (retry_timeout <= 15) ) &&
		  ( (retry_count >= 0) && (retry_count <= 15) )    ) {
  	LMark;

		radio.setRetries(retry_timeout, retry_count);

		print_module_label();
		LPrintln("\tRetry delay: ", 250*retry_timeout, " microseconds");
		LPrintln("\tRetry count: ", retry_count);
	}

	// Begin network manager with provided address
	network->begin(90, address); // determine good way to convert change to octal address?
}

///////////////////////////////////////////////////////////////////////////////
nRF::nRF(JsonArrayConst p)
	: nRF(EXPAND_ARRAY(p, 8) ) {}

///////////////////////////////////////////////////////////////////////////////
nRF::~nRF()
{
  LMark;
	delete network;
// 	delete radio;
}

///////////////////////////////////////////////////////////////////////////////
void nRF::print_config() const
{
	CommPlat::print_config();

	LPrintln("\tAddress          : ", address );
	LPrintln("\tData Rate        : ", data_rate );
	LPrintln("\tPower Level      : ", power_level );
	LPrintln("\tRetry Count      : ", retry_count );
	LPrintln("\tRetry Timeout    : ", retry_timeout );
}

///////////////////////////////////////////////////////////////////////////////
bool nRF::receive_blocking_impl(JsonObject json, uint max_wait_time)
{
	bool status = false;
	const unsigned long start_time = millis();
	do {
   	LMark;
		network->update();                      // Check the network regularly

		while ( network->available() ) {        // Is there anything ready for us?
			RF24NetworkHeader header;          // If so, grab it and print it out
			char buffer[max_message_len];
			memset(buffer, '\0', max_message_len);
			if (network->read(header, &buffer, max_message_len-1) )
				LMark;
				status = msgpack_buffer_to_json(buffer, json);
		}
	} while ( (millis() - start_time) < max_wait_time );

	print_module_label();
	LPrintln("Receive ", (status) ? "successful" : "failed");
	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool nRF::send_impl(JsonObject json, const uint8_t destination)
{
	char buffer[max_message_len];
  LMark;
	bool to_msgpack = json_to_msgpack_buffer(json, buffer, max_message_len);
	if (!to_msgpack) return false;

	RF24NetworkHeader header(destination);
  LMark;
	bool is_sent = network->write( header, buffer, measureMsgPack(json) );
	// bool is_sent = network->write( header, buffer, 120 );

	print_module_label();
	LPrintln("Send " , (is_sent) ? "successful" : "failed" );
	return is_sent;
}

///////////////////////////////////////////////////////////////////////////////
void nRF::broadcast_impl(JsonObject json)
{
	char buffer[max_message_len];
  LMark;
	bool to_msgpack = json_to_msgpack_buffer(json, buffer, max_message_len);
	if (!to_msgpack) {
		print_module_label();
		LPrintln("Failed to convert to msgpack");
		return;
	}

	RF24NetworkHeader header(00);
  LMark;
	network->multicast( header, buffer, measureMsgPack(json), multicast_level );

	print_module_label();
	LPrintln("Broadcasted");
}

///////////////////////////////////////////////////////////////////////////////
void nRF::set_address(const uint8_t addr)    // Need to test this
{
  LMark;
	address = addr;
	network->begin(90, address);
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_RADIOS
