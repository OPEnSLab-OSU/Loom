///////////////////////////////////////////////////////////////////////////////
///
/// @file		CommPlat.cpp
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
		const char*							module_name,
		const uint16_t					max_message_len,
		const bool							override_name
	)
	: Module(module_name)
	, max_message_len(max_message_len)
	, signal_strength(0)
	, total_packet_count(0)
	, total_drop_count(0)
	, last_ten_dropped{}
	, last_ten_dropped_idx(0)
	, mergeJson(2048)
	, override_name(override_name)
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
  LMark;
	return (total_packet_count == 0)
		? 0.0f
		: static_cast<float>(total_drop_count)*100.0f/static_cast<float>(total_packet_count);
}

///////////////////////////////////////////////////////////////////////////////
float CommPlat::get_last_ten_drop_rate() const
{
	uint8_t total = 0;
	uint8_t min_pak = static_cast<uint8_t>(total_packet_count < 10U ? total_packet_count : 10U);
	for (uint8_t i = 0; i < min_pak; i++){
		if (last_ten_dropped[i])
			total++;
	}
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

	// If there is a value called "Num_Package", then it will recognize that there are more packages
	JsonObject checker = device_manager -> internal_json();
	if(!(checker["Num_Package"].isNull())){
		status = merge_json(pre_merge_receive_blocking(checker), checker["Num_Package"]);
	}

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
JsonObject CommPlat::pre_merge_receive_blocking(JsonObject json){

	// This will get the timestamp(if RTC is used), device id, and other information that is not part of the contents array
	JsonObject newJson = mergeJson.to<JsonObject>();
	newJson["type"] = json["type"];
	JsonObject information = newJson.createNestedObject("id");
	information["name"] = json["id"]["name"];
	information["instance"] = json["id"]["instance"];
	if(!(json["timestamp"].isNull())){
		JsonObject timestamp = newJson.createNestedObject("timestamp");
		timestamp["date"] = json["timestamp"]["date"];
		timestamp["time"] = json["timestamp"]["time"];
	}
	// Note that this json doesn't have the Num_Package value
	return newJson;


}

///////////////////////////////////////////////////////////////////////////////
bool CommPlat::merge_json(JsonObject json, const uint8_t loop){

	// In the json, it will create contents jsonarray to add the upcoming small packages
	JsonArray newContents = json["contents"];
	newContents = json.createNestedArray("contents");

	// Loop value is determine by "Num_Package" value
	uint8_t Loop = loop;

	bool increased = false;


	while(Loop > 0){

		// Receive a package from the other board
		bool status = receive_blocking_impl(device_manager -> internal_json(true), 1000);

		// If it fails at least once, return false
		if(!status){
			Loop = 0;
			return false;
		}

		JsonObject compenent = newContents.createNestedObject();
		compenent["module"] = device_manager -> internal_json()["contents"]["module"];
		JsonObject data = compenent.createNestedObject("data");
		JsonObject old_data = device_manager -> internal_json()["contents"]["data"];
		for(JsonPair kv: old_data){
			compenent["data"][kv.key()] = kv.value();
		}
		Loop--;
	}

	// Once the json is complete, change the internal_json to the big json
	// It can't reach the full capacity of sending and receiving because of deserializeMsgPack takes up memory
	// Need to find a solution so that it can take even bigger pacakge than before. Todo!
	// For now, we will let the user if there going to be package drops or not in the if statment

	if(json.memoryUsage() >= 2000){
		print_module_label();
		LPrintln("Some packages will be dropped during the tranmission");
		print_module_label();
		LPrintln("Reduce the size you are sending by this amount of bytes: ", json.memoryUsage() - 2000);
	}

	device_manager -> internal_json(true).set(json);
	mergeJson.clear();
	return true;

}

///////////////////////////////////////////////////////////////////////////////
bool CommPlat::receive_batch_blocking(uint max_wait_time){
	bool receive_results = false;
	BatchSD* batch;
	if (device_manager && (batch = device_manager->get<BatchSD>()) ) {
		receive_results = receive_blocking(max_wait_time);
		if (!receive_results) {
			return false;
		}
		return batch->store_batch();
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
bool	CommPlat::send(JsonObject json, const uint8_t destination) {

	char buffer[max_message_len];
	uint16_t sizeJsonObject = serializeMsgPack(json, buffer, max_message_len);
	bool prestatus;
	bool status;

	// If json package size is over 252, then send into multiple packages
	if (sizeJsonObject >= 252){
		prestatus = split_send_notification(json, destination);
		if (prestatus) status = split_send(json, destination, 0);
		else{
			status = false;
		}
	}
	// Else, just send as it is
	else{
		status = send_impl(json, destination);
	}
	add_packet_result(!status);
	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool CommPlat::send(const uint8_t destination)
{
  LMark;
	if (device_manager != nullptr) {
   	LMark;
		JsonObject tmp = device_manager->internal_json();
   	LMark;
		if (strcmp(tmp["type"], "data") == 0 )
			return send(tmp, destination);
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
uint8_t CommPlat::send_batch(const uint8_t destination, int delay_time){
	BatchSD* batch;
	// check to make sure we have BatchSD module connected
	if (device_manager && (batch = device_manager->get<BatchSD>()) ) {
		uint8_t drop_count = 0;
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
uint16_t CommPlat::determine_json_size(JsonObject json){

	// Calculate the size of the JSON Package
	// https://arduinojson.org/v6/assistant/

	// Calculate the Total Number of JSON Object (Outside)
	uint16_t jsonObjectSize = JSON_OBJECT_SIZE(json.size());

	// Calculate Internal Json from the Object id
	jsonObjectSize += JSON_OBJECT_SIZE(json["id"].size());

	// If there is a timestamp(using RTC), then add that space
	if (!(json["timestamp"].isNull())){
		jsonObjectSize +=JSON_OBJECT_SIZE(json["timestamp"].size());
	}

	// Add Number of Element in Array and Module name Object per Element in the Array
	jsonObjectSize += JSON_ARRAY_SIZE(json["contents"].size()) + JSON_OBJECT_SIZE(json["contents"].size());

	// Add Data Object per module
	int index = 0;
	while(!(json["contents"][index]["data"].isNull())){
		jsonObjectSize += JSON_OBJECT_SIZE(json["contents"][index]["data"].size());
		index += 1;
	}

	return jsonObjectSize;

}

///////////////////////////////////////////////////////////////////////////////
bool CommPlat::split_send_notification(JsonObject json, const uint8_t destination) {


	// This process will copy information about device id, and timestamp from the original package
	uint8_t numPackage = json["contents"].size();
	JsonObject object = mergeJson.to<JsonObject>();
	object["type"] = json["type"];
	JsonObject information = object.createNestedObject("id");
	information["name"] = json["id"]["name"];
	information["instance"] = json["id"]["instance"];
	if(!(json["timestamp"].isNull())){
		JsonObject timestamp = object.createNestedObject("timestamp");
		timestamp["date"] = json["timestamp"]["date"];
		timestamp["time"] = json["timestamp"]["time"];
	}
	// Create a small json package that have the information about the upcoming package number
	object["Num_Package"] = numPackage;
	LPrintln("Sending ", numPackage, " more package to the other board due to the size of the package");
	// Sending this small package to the other board
	bool status = send_impl(object, destination);

	mergeJson.clear();
	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool CommPlat::split_send(JsonObject json, const uint8_t destination, const uint8_t index){

	// This will send each module from the contents array
	int contentIndex = index;

	mergeJson.clear();

	JsonObject tmp = mergeJson.to<JsonObject>();
	JsonObject compenent = tmp.createNestedObject("contents");

	compenent["module"] = json["contents"][contentIndex]["module"];
	JsonObject data = compenent.createNestedObject("data");

	JsonObject old_data = json["contents"][contentIndex]["data"];
	for (JsonPair kv : old_data){
		compenent["data"][kv.key()] = kv.value();
	}

	// Send the small package
	bool status = send_impl(tmp, destination);
	if(!status) return false;

	contentIndex++;
	// If there no more contents in the original json package, then it will return true
	if(json["contents"][contentIndex].isNull()) return true;
	// else, it will keep creating and sending small json packages

	split_send(json, destination, contentIndex);

}

///////////////////////////////////////////////////////////////////////////////
void CommPlat::broadcast()
{
	if (device_manager != nullptr) {
		JsonObject tmp = device_manager->internal_json();
		if (strcmp(tmp["type"], "data") == 0 ) {
    	LMark;
			broadcast(tmp);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void CommPlat::broadcast_batch(int delay_time)
{
	BatchSD* batch;
	// check to make sure we have BatchSD module connected
	if (device_manager && (batch = device_manager->get<BatchSD>()) ) {
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
  LMark;

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
		LPrintln("Received Json Memory Usage: ", measureMsgPack(messageJson));
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
	LMark;
	if(override_name) strcpy(device_manager->temp_device_name, json["id"]["name"]);
	return status;
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
