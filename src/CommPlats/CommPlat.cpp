///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_CommPlat.cpp
/// @brief		File for LoomCommPlat implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "CommPlat.h"
#include "Manager.h"

///////////////////////////////////////////////////////////////////////////////
LoomCommPlat::LoomCommPlat( 
		LoomManager* 			manager,
		const char*							module_name, 
		const LoomModule::Type	module_type,
		const uint16_t					max_message_len 
	) 
	: LoomModule(manager, module_name, module_type )
	, max_message_len(max_message_len)
	, signal_strength(0)
	, total_packet_count(0)
	, total_drop_count(0)
	, last_ten_dropped{}
	, last_ten_dropped_idx(0)
{}

///////////////////////////////////////////////////////////////////////////////
void LoomCommPlat::print_config() const
{
	LoomModule::print_config();
	LPrintln("\tMax Message Length  : ", max_message_len );
}

///////////////////////////////////////////////////////////////////////////////
void LoomCommPlat::print_state() const
{
	LoomModule::print_state();
	LPrintln("\tDrop Rate Since Start  : ", get_drop_rate() );
	LPrintln("\tCurrent Drop Rate  : ", get_last_ten_drop_rate() );
}

///////////////////////////////////////////////////////////////////////////////
float LoomCommPlat::get_drop_rate() const
{
	return (total_packet_count == 0)
		? 0.0f
		: static_cast<float>(total_drop_count)*100.0f/static_cast<float>(total_packet_count);
}

///////////////////////////////////////////////////////////////////////////////
float LoomCommPlat::get_last_ten_drop_rate() const
{
	uint8_t total = 0;
	uint8_t min_pak = static_cast<uint8_t>(total_packet_count < 10U ? total_packet_count : 10U);
	for (uint8_t i = 0; i < min_pak; i++)
		if (last_ten_dropped[i])
			total++;
	return static_cast<float>(total)*100.0f/static_cast<float>(min_pak);
}

///////////////////////////////////////////////////////////////////////////////
bool LoomCommPlat::receive()
{
	if (device_manager != nullptr) {
		// Loom_Manager's json needs to be cleared (passing true to internal_json)
		// in order to copy over correctly
		return receive( device_manager->internal_json(true) );
	} 
	return false;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomCommPlat::receive_blocking(JsonObject json, const uint max_wait_time)
{
	bool status = receive_blocking_impl(json, max_wait_time);
	LPrintln("Recieve " , (status) ? "successful" : "failed" );
	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomCommPlat::receive_blocking(const uint max_wait_time)
{
	if (device_manager != nullptr) {
		// Loom_Manager's json needs to be cleared (passing true to internal_json)
		// in order to copy over correctly
		return receive_blocking( device_manager->internal_json(true), max_wait_time );
	} 
	return false;
}

///////////////////////////////////////////////////////////////////////////////
bool	LoomCommPlat::send(JsonObject json, const uint8_t destination) { 
	bool status = send_impl(json, destination);
	add_packet_result(!status);
	LPrintln("Send " , (status) ? "successful" : "failed" );
	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomCommPlat::send(const uint8_t destination)
{
	if (device_manager != nullptr) {
		JsonObject tmp = device_manager->internal_json();
		if (strcmp(tmp["type"], "data") == 0 )
			return send(tmp, destination);
	} 
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void LoomCommPlat::broadcast()
{
	if (device_manager != nullptr) {
		JsonObject tmp = device_manager->internal_json();
		if (strcmp(tmp["type"], "data") == 0 ) {
			broadcast(tmp);
		}
	} 
}

///////////////////////////////////////////////////////////////////////////////
bool LoomCommPlat::json_to_msgpack_buffer(JsonObjectConst json, char* buffer, const uint16_t max_len) const
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
bool LoomCommPlat::msgpack_buffer_to_json(const char* buffer, JsonObject json)
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
void LoomCommPlat::add_packet_result(const bool did_drop) {
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
