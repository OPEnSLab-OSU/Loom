
#include "Loom_CommPlat.h"
#include "Loom_Manager.h"


///////////////////////////////////////////////////////////////////////////////
const char* LoomCommPlat::enum_comm_plat_string(Platform c)
{
	switch(c) {
		case Platform::WIFI : return "WiFi";
		case Platform::LORA : return "LoRa";
		case Platform::NRF  : return "nRF";
		case Platform::SSER : return "SlipSerial";	    
	}
}

///////////////////////////////////////////////////////////////////////////////
LoomCommPlat::LoomCommPlat( 
		const char*		module_name, 
		uint			max_message_len 
	) 
	: LoomModule( module_name )
{
	this->max_message_len = max_message_len;
	this->signal_strength = 0;
}

///////////////////////////////////////////////////////////////////////////////
void LoomCommPlat::print_config() 
{
	LoomModule::print_config();

	LPrintln('\t', "Max Message Length  : ", max_message_len );
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
bool LoomCommPlat::send(uint16_t destination)
{
	if (device_manager != nullptr) {
		JsonObject tmp = device_manager->internal_json();
		if (strcmp(tmp["type"], "data") == 0 ) {
			return send(tmp, destination);
		}
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
bool LoomCommPlat::json_to_msgpack_buffer(JsonObjectConst json, char* buffer, int max_len)
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
		LPrintln("\nInternal messageJson:");
		serializeJsonPretty(messageJson, Serial);

		// LPrintln("\nJson passed in:");
		// serializeJsonPretty(json, Serial);
		LPrintln();
	}
}

///////////////////////////////////////////////////////////////////////////////



