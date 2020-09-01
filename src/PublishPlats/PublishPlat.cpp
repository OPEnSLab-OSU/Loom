///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_PublishPlat.cpp
/// @brief		File for LoomPublishPlat implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "PublishPlat.h"
#include "Manager.h"


///////////////////////////////////////////////////////////////////////////////
LoomPublishPlat::LoomPublishPlat(
		LoomManager* 			manager,
		const char*							module_name,
		const LoomModule::Type	module_type,
		const LoomModule::Type	internet_type
	)
	: LoomModule(manager, module_name, module_type )
	, m_internet( nullptr )
	, internet_type( internet_type )
{}

///////////////////////////////////////////////////////////////////////////////
void LoomPublishPlat::second_stage_ctor()
{
	// check to see if we have a device manager
	if (device_manager == nullptr) {
		print_module_label();
		LPrint("No Device Manager!\n");
		return;
	}

	// check if internet platform exist
	LoomInternetPlat* temp;
	switch (internet_type) {
		case LoomModule::Type::Ethernet:
			temp = (LoomInternetPlat*)&(device_manager->Ethernet() );
			break;
		case LoomModule::Type::WiFi:
			temp = (LoomInternetPlat*)&(device_manager->WiFi() );
			break;
		case LoomModule::Type::LTE:
			temp = (LoomInternetPlat*)&(device_manager->LTE() );
			break;
		default:
			temp = nullptr;
	}


	print_module_label();
	// if (temp->category() == LoomModule::Category::InternetPlat) {
	if (temp != nullptr && temp->get_module_type() != LoomModule::Type::Unknown) {
		LPrintln("Found internet module: ", temp->get_module_name() , " (", (int)temp->get_module_type() , ")");
		// m_internet = temp;
		m_internet = temp;
	}
	else {
		LPrintln("Unable to find internet platform");
		return;
	}

	// made it here, guess we're good to go!
	print_module_label();
	LPrint("Ready\n");
}

///////////////////////////////////////////////////////////////////////////////
bool LoomPublishPlat::publish(const JsonObject json)
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
uint8_t LoomPublishPlat::publish_batch()
{
	// check to make sure we have BatchSD module connected
	if(device_manager != nullptr && device_manager->has_module(LoomModule::Type::BATCHSD)){
		// retrieve the Batch SD module
		uint8_t drop_count = 0;
		Loom_BatchSD* batch = (Loom_BatchSD*)device_manager->find_module(LoomModule::Type::BATCHSD);
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
bool LoomPublishPlat::publish()
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
void LoomPublishPlat::print_config() const
{
	LoomModule::print_config();
	LPrintln("\tInternet Type: ", (int)internet_type);
}

///////////////////////////////////////////////////////////////////////////////
void LoomPublishPlat::print_state() const
{
	LoomModule::print_state();
	LPrintln("\tInternet Connected: ", m_internet != nullptr && m_internet->is_connected());
}

///////////////////////////////////////////////////////////////////////////////
bool LoomPublishPlat::m_validate_json(const JsonObjectConst json) const
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
void LoomPublishPlat::m_print_json_error(const char* str) const
{
	print_module_label();
	LPrint("Publish data is invalid: ", str, "\n");
}

///////////////////////////////////////////////////////////////////////////////
