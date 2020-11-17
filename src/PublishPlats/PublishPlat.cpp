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
  LMark;
	// check to see if we have a device manager
	if (device_manager == nullptr) {
   	LMark;
		print_module_label();
   	LMark;
		LPrint("No Device Manager!\n");
   	LMark;
		return;
	}

	// check if internet platform exist
	LoomInternetPlat* temp;
  LMark;
	switch (internet_type) {
		case LoomModule::Type::Ethernet:
    	LMark;
			temp = (LoomInternetPlat*)&(device_manager->Ethernet() );
    	LMark;
			break;
		case LoomModule::Type::WiFi:
    	LMark;
			temp = (LoomInternetPlat*)&(device_manager->WiFi() );
    	LMark;
			break;
		case LoomModule::Type::LTE:
    	LMark;
			temp = (LoomInternetPlat*)&(device_manager->LTE() );
    	LMark;
			break;
		default:
    	LMark;
			temp = nullptr;
  		LMark;
	}


	print_module_label();
  LMark;
	// if (temp->category() == LoomModule::Category::InternetPlat) {
	if (temp != nullptr && temp->get_module_type() != LoomModule::Type::Unknown) {
   	LMark;
		LPrintln("Found internet module: ", temp->get_module_name() , " (", (int)temp->get_module_type() , ")");
   	LMark;
		// m_internet = temp;
		m_internet = temp;
  	LMark;
	}
	else {
   	LMark;
		LPrintln("Unable to find internet platform");
   	LMark;
		return;
	}

	// made it here, guess we're good to go!
	print_module_label();
  LMark;
	LPrint("Ready\n");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomPublishPlat::publish(const JsonObject json)
{
  LMark;
	// check validity
	if (m_internet == nullptr  || !m_validate_json(json)){
   	LMark;
		print_module_label();
   	LMark;
		LPrint("Could not publish without ");
   	LMark;
		if(m_internet == nullptr) LPrint("internet module\n");
		else LPrint("valid data.\n");
   	LMark;
		return false;
	}
	// guess we're good to go
	return send_to_internet(json, m_internet);
}

///////////////////////////////////////////////////////////////////////////////
uint8_t LoomPublishPlat::publish_batch()
{
  LMark;
	// check to make sure we have BatchSD module connected
	if(device_manager != nullptr && device_manager->has_module(LoomModule::Type::BATCHSD)){
   	LMark;
		// retrieve the Batch SD module
		uint8_t drop_count = 0;
   	LMark;
		Loom_BatchSD* batch = (Loom_BatchSD*)device_manager->find_module(LoomModule::Type::BATCHSD);
   	LMark;
		int packets = batch->get_packet_counter();
   	LMark;
		print_module_label();
   	LMark;
		LPrintln("Packets in batch to publish: ", packets);
   	LMark;
		JsonObject tmp;
   	LMark;
		// For all the jsons stored in the batch, run the publish function using the json
		for(int i=0; i < packets; i++){
    	LMark;
			tmp = batch->get_batch_json(i);
    	LMark;
			if(!publish(tmp)) drop_count++;
    	LMark;
			//delay(1000);
		}
		// Clear the batch for the next batching to start
		batch->clear_batch_log();
   	LMark;
		batch->add_drop_count(drop_count);
   	LMark;
		return drop_count;
	}
	return -1;
}


///////////////////////////////////////////////////////////////////////////////
bool LoomPublishPlat::publish()
{
  LMark;
	if (device_manager != nullptr) {
   	LMark;
		JsonObject tmp = device_manager->internal_json();
   	LMark;
		if (strcmp(tmp["type"], "data") == 0 ) {
    	LMark;
			return publish(tmp);
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void LoomPublishPlat::print_config() const
{
  LMark;
	LoomModule::print_config();
  LMark;
	LPrintln("\tInternet Type: ", (int)internet_type);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomPublishPlat::print_state() const
{
  LMark;
	LoomModule::print_state();
  LMark;
	LPrintln("\tInternet Connected: ", m_internet != nullptr && m_internet->is_connected());
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomPublishPlat::m_validate_json(const JsonObjectConst json) const
{
  LMark;
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
   	LMark;
		bool valid = true;
   	LMark;
		const JsonArrayConst ray = json["contents"];
   	LMark;
		for (const auto elem : ray) {
    	LMark;
			if (!elem["module"].is<const char*>()) {
     		LMark;
				valid = false;
     		LMark;
				m_print_json_error("contents array does not contain objects with a module key");
     		LMark;
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
  LMark;
	print_module_label();
  LMark;
	LPrint("Publish data is invalid: ", str, "\n");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
