///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_CommPlat.cpp
/// @brief		File for CommPlat implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_RADIOS

#include "CommPlat.h"
#include "Manager.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
CommPlat::CommPlat(
		const char*				module_name,
		const Module::Type	module_type,
		const uint16_t			max_message_len
	)
	: Module(module_name, module_type)
	, max_message_len(max_message_len)
	, signal_strength(0)
	, total_packet_count(0)
	, total_drop_count(0)
	, last_ten_dropped{}
	, last_ten_dropped_idx(0)
{}

///////////////////////////////////////////////////////////////////////////////
void CommPlat::print_config() const
{
	Module::print_config();
	LPrintln("\tMax Message Length  : ", max_message_len );
}

///////////////////////////////////////////////////////////////////////////////
void CommPlat::print_state() const
{
	Module::print_state();
	LPrintln("\tDrop Rate Since Start  : ", get_drop_rate() );
	LPrintln("\tCurrent Drop Rate  : ", get_last_ten_drop_rate() );
}

///////////////////////////////////////////////////////////////////////////////
float CommPlat::get_drop_rate() const
{
	return (total_packet_count == 0)
		? 0.0f
		: static_cast<float>(total_drop_count)*100.0f/static_cast<float>(total_packet_count);
}

///////////////////////////////////////////////////////////////////////////////
float CommPlat::get_last_ten_drop_rate() const
{
	uint8_t total = 0;
	uint8_t min_pak = static_cast<uint8_t>(total_packet_count < 10U ? total_packet_count : 10U);
	for (uint8_t i = 0; i < min_pak; i++)
		if (last_ten_dropped[i])
			total++;
	return static_cast<float>(total)*100.0f/static_cast<float>(min_pak);
}

///////////////////////////////////////////////////////////////////////////////
bool CommPlat::receive()
{
	if (device_manager != nullptr) {
		// Loom_Manager's json needs to be cleared (passing true to internal_json)
		// in order to copy over correctly
		return receive( device_manager->internal_json(true) );
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
bool CommPlat::receive_blocking(JsonObject json, const uint max_wait_time)
{
	bool status = receive_blocking_impl(json, max_wait_time);
	LPrintln("Recieve " , (status) ? "successful" : "failed" );
	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool CommPlat::receive_blocking(const uint max_wait_time)
{
	if (device_manager != nullptr) {
		// Loom_Manager's json needs to be cleared (passing true to internal_json)
		// in order to copy over correctly
		return receive_blocking( device_manager->internal_json(true), max_wait_time );
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
bool CommPlat::receive_batch_blocking(uint max_wait_time){
	bool receive_results=false;
	if(device_manager != nullptr && device_manager->has_module(Module::Type::BATCHSD)){
		BatchSD* batch = (BatchSD*)device_manager->find_module(Module::Type::BATCHSD);
		receive_results = receive_blocking(max_wait_time);
		if(!receive_results) return false;
		return batch->store_batch();
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
bool	CommPlat::send(JsonObject json, const uint8_t destination) {
	bool status = send_impl(json, destination);
	add_packet_result(!status);
	LPrintln("Send " , (status) ? "successful" : "failed" );
	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool CommPlat::send(const uint8_t destination)
{
	if (device_manager != nullptr) {
		JsonObject tmp = device_manager->internal_json();
		if (strcmp(tmp["type"], "data") == 0 )
			return send(tmp, destination);
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
uint8_t CommPlat::send_batch(const uint8_t destination, int delay_time){
	// check to make sure we have BatchSD module connected
	if(device_manager != nullptr && device_manager->has_module(Module::Type::BATCHSD)){
		// retrieve the Batch SD module
		uint8_t drop_count = 0;
		BatchSD* batch = (BatchSD*)device_manager->find_module(Module::Type::BATCHSD);
		int packets = batch->get_packet_counter();
		print_module_label();
		LPrintln("Packets to send: ", packets);
		JsonObject tmp;
		// For all the jsons stored in the batch, run the sebd function using the json
		for(int i=0; i < packets; i++){
			tmp = batch->get_batch_json(i);
			if(!send(tmp, destination)) drop_count++;;
			device_manager->pause(delay_time);
		}
		// Clear the batch for the next batching to start
		batch->clear_batch_log();
		batch->add_drop_count(drop_count);
		return drop_count;
	}
	return -1;
}

///////////////////////////////////////////////////////////////////////////////
void CommPlat::broadcast()
{
	if (device_manager != nullptr) {
		JsonObject tmp = device_manager->internal_json();
		if (strcmp(tmp["type"], "data") == 0 ) {
			broadcast(tmp);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void CommPlat::broadcast_batch(int delay_time)
{
	// check to make sure we have BatchSD module connected
	if(device_manager != nullptr && device_manager->has_module(Module::Type::BATCHSD)){
		// retrieve the Batch SD module
		BatchSD* batch = (BatchSD*)device_manager->find_module(Module::Type::BATCHSD);
		int packets = batch->get_packet_counter();
		print_module_label();
		LPrintln("Packets to broadcast: ", packets);
		JsonObject tmp;
		// For all the jsons stored in the batch, run the sebd function using the json
		for(int i=0; i < packets; i++){
			tmp = batch->get_batch_json(i);
			broadcast(tmp);
			device_manager->pause(delay_time);
		}
		// Clear the batch for the next batching to start
		batch->clear_batch_log();
	}
}


///////////////////////////////////////////////////////////////////////////////
bool CommPlat::json_to_msgpack_buffer(JsonObjectConst json, char* buffer, const uint16_t max_len) const
{
	memset(buffer, '\0', sizeof(buffer));

	bool status = serializeMsgPack(json, buffer, max_len);

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln(buffer);
		LPrintln("MsgPack size: ", measureMsgPack(json));
	}

	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool CommPlat::msgpack_buffer_to_json(const char* buffer, JsonObject json)
{
	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("Received: ", (const char*)buffer);
		print_module_label();
		LPrintln("Len: ", strlen(buffer));
	}

	messageJson.clear();

	if (deserializeMsgPack(messageJson, buffer) != DeserializationError::Ok ) {
		print_module_label();
		LPrintln("Failed to parse MsgPack");
		return false;
	}

	bool status = json.set(messageJson.as<JsonObject>());
	if (!status) return false;

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("Internal messageJson:");
		serializeJsonPretty(messageJson, Serial);
		LPrintln();

		// LPrintln("\nJson passed in:");
		// serializeJsonPretty(json, Serial);
	}
}

///////////////////////////////////////////////////////////////////////////////
void CommPlat::add_packet_result(const bool did_drop) {
	// shift last_ten_dropped by one
	last_ten_dropped[last_ten_dropped_idx++] = did_drop;
	if (last_ten_dropped_idx >= 10)
		last_ten_dropped_idx = 0;
	// add one to the packet total, and maybe one to the drop count
	total_packet_count++;
	if (did_drop)
		total_drop_count++;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_RADIOS

