///////////////////////////////////////////////////////////////////////////////
///
/// @file		PublishPlat.cpp
/// @brief		File for PublishPlat implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))

#include "PublishPlat.h"
#include "Manager.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////

// class Etherent;
// class WiFi;
// class LTE:

///////////////////////////////////////////////////////////////////////////////
PublishPlat::PublishPlat(
		const char* module_name
	)
	: Module(module_name)
	, m_internet( nullptr )
{}

///////////////////////////////////////////////////////////////////////////////
void PublishPlat::second_stage_ctor()
{
	// check to see if we have a device manager
	if (!device_manager) {
		print_module_label();
		LPrint("No Device Manager!\n");
		return;
	}

	// Check for InternetPlat, in preference of
	// Ethernet > WiFi > LTE
	// Else set to nullptr

	// If which InternetPlat is used in the event of multiple being present
	// the below code could make use of a single 
	// `if (temp = device_manager->get<InternetPlat>())`

	// InternetPlat* temp;
	// // Note the single equal signs are intentional
	// if (temp = device_manager->get<Loom::Ethernet>()) {
	// 	LPrintln("Found Etherent module");
	// } 
	// else if (temp = device_manager->get<Loom::WiFi>()) {
	// 	LPrintln("Found WiFi module");
	// }
	// else if (temp = device_manager->get<Loom::LTE>()) {
	// 	LPrintln("Found LTE module");
	// } else {
	// 	LPrintln("Unable to find internet platform");
	// 	temp = nullptr;
	// }

	m_internet = device_manager->get<InternetPlat>();
	if (!m_internet) {
		print_module_label();
		LPrintln("Unable to find internet platform");
	}

	// made it here, guess we're good to go!
	print_module_label();
	LPrint("Ready\n");
}

///////////////////////////////////////////////////////////////////////////////
bool PublishPlat::publish(const JsonObject json)
{
	// check validity
	if (m_internet == nullptr  || !m_validate_json(json)){
		print_module_label();
		LPrint("Could not publish without ");
		if(m_internet == nullptr) LPrint("internet module\n");
		else LPrint("valid data.\n");
		return false;
	}
	// guess we're good to go
	return send_to_internet(json, m_internet);
}

///////////////////////////////////////////////////////////////////////////////
uint8_t PublishPlat::publish_batch()
{
	// // check to make sure we have BatchSD module connected
	// if(device_manager != nullptr && device_manager->has_module(Module::Type::BATCHSD)){
	// 	// retrieve the Batch SD module
	// 	uint8_t drop_count = 0;
	// 	BatchSD* batch = (BatchSD*)device_manager->find_module(Module::Type::BATCHSD);
	// 	int packets = batch->get_packet_counter();
	// 	print_module_label();
	// 	LPrintln("Packets in batch to publish: ", packets);
	// 	JsonObject tmp;
	// 	// For all the jsons stored in the batch, run the publish function using the json
	// 	for(int i=0; i < packets; i++){
	// 		tmp = batch->get_batch_json(i);
	// 		if(!publish(tmp)) drop_count++;
	// 		//delay(1000);
	// 	}
	// 	// Clear the batch for the next batching to start
	// 	batch->clear_batch_log();
	// 	batch->add_drop_count(drop_count);
	// 	return drop_count;
	// }
	// return -1;

	BatchSD* batch;
	// check to make sure we have BatchSD module connected
	if (device_manager && (batch = device_manager->get<BatchSD>()) ) {
		uint8_t drop_count = 0;
		int packets = batch->get_packet_counter();
		print_module_label();
		LPrintln("Packets in batch to publish: ", packets);
		JsonObject tmp;
		// For all the jsons stored in the batch, run the publish function using the json
		for(int i=0; i < packets; i++){
			tmp = batch->get_batch_json(i);
			if(!publish(tmp)) drop_count++;
			//delay(1000);
		}
		// Clear the batch for the next batching to start
		batch->clear_batch_log();
		batch->add_drop_count(drop_count);
		return drop_count;
	}
	return -1;
}


///////////////////////////////////////////////////////////////////////////////
bool PublishPlat::publish()
{
	if (device_manager != nullptr) {
		JsonObject tmp = device_manager->internal_json();
		if (strcmp(tmp["type"], "data") == 0 ) {
			return publish(tmp);
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void PublishPlat::print_config() const
{
	Module::print_config();
	// LPrintln("\tInternet Type: ", (int)internet_type);
}

///////////////////////////////////////////////////////////////////////////////
void PublishPlat::print_state() const
{
	Module::print_state();
	LPrintln("\tInternet Connected: ", m_internet != nullptr && m_internet->is_connected());
}

///////////////////////////////////////////////////////////////////////////////
bool PublishPlat::m_validate_json(const JsonObjectConst json) const
{
	// check if we got an object at all
	if (json.isNull())
		m_print_json_error("root json is null");
	// check that a timestamp exists, and if it does ensure that it's an object
	else if (!json["timestamp"].isNull() && !json["timestamp"].is<JsonObject>())
		m_print_json_error("timestamp is not object");
	// check if the contents exist and is an array
	else if (json["contents"].isNull() || !json["contents"].is<JsonArray>())
		m_print_json_error("contents is not an array");
	// check that every item in contents is an object with a module name
	else {
		bool valid = true;
		const JsonArrayConst ray = json["contents"];
		for (const auto elem : ray) {
			if (!elem["module"].is<const char*>()) {
				valid = false;
				m_print_json_error("contents array does not contain objects with a module key");
				break;
			}
		}
		return valid;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void PublishPlat::m_print_json_error(const char* str) const
{
	print_module_label();
	LPrint("Publish data is invalid: ", str, "\n");
}

///////////////////////////////////////////////////////////////////////////////

#endif // if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
