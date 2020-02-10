///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Max_Sub.cpp
/// @brief		File for Loom_MaxSub implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Max_Sub.h"

#include "../Manager.h"

///////////////////////////////////////////////////////////////////////////////
Loom_MaxSub::Loom_MaxSub(
		LoomManager* manager,
		const LoomModule::Type	internet_type,
		const bool				auto_dispatch
	)   
	: LoomSubscribePlat(manager, "MaxSub", Type::MaxSub, internet_type )
	, auto_dispatch(auto_dispatch)
{}

///////////////////////////////////////////////////////////////////////////////
Loom_MaxSub::Loom_MaxSub(LoomManager* manager, JsonArrayConst p) 
	: Loom_MaxSub(manager, (LoomModule::Type)(int)p[0], p[1] ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxSub::second_stage_ctor() 
{
	LoomSubscribePlat::second_stage_ctor();

	UDP_port = UDP_RECEIVE_OFFSET + ((device_manager) ? device_manager->get_instance_num() : 0);

	// Get new UDP pointer	
	if (m_internet != nullptr) {
		UDP_Inst = m_internet->open_socket(UDP_port);
	} else {
		print_module_label();
		LPrintln("No internet platform, could not get UDP object");
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_MaxSub::subscribe(JsonObject json)
{
	// Make sure UDP object exists
	if (!UDP_Inst) {
		// Try to get UDP 
		UDP_Inst = (m_internet != nullptr) ? m_internet->open_socket(UDP_port) : nullptr;
		// Check if still null
		if (!UDP_Inst) {
			LPrintln("UDP_Inst is null");
			return false;
		}
	}

	// If packet available
	if ( UDP_Inst->parsePacket() ) {

		messageJson.clear();
		if ( deserializeJson(messageJson, (*UDP_Inst) ) != DeserializationError::Ok ) {
			print_module_label();
			LPrintln("Failed to parse MsgPack");
			return false;
		}

		bool status = json.set(messageJson.as<JsonObject>());
		if (!status) {
			LPrintln("Json set error");	
			return false;
		}

		if (print_verbosity == Verbosity::V_HIGH) {
			print_module_label();
			LPrint("From IP: ");
			IPAddress remote = UDP_Inst->remoteIP();
			for (auto i=0; i < 4; i++) {
				LPrint(remote[i]);
				if (i < 3) {
					LPrint(".");
				} else {
					LPrintln();
				}
			}

			print_module_label();
			LPrintln("Internal messageJson:");
			serializeJsonPretty(messageJson, Serial);
			LPrintln();
		}

		// If auto dispatch, have device manager dispatch command
		if (auto_dispatch && device_manager != nullptr) {
			device_manager->dispatch();
		}

		return true;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxSub::print_config() const
{
	LoomSubscribePlat::print_config();
	LPrintln("\tUDP Port : ", UDP_port);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxSub::set_port(const uint16_t port)
{
	UDP_port = port;

	if (m_internet != nullptr) {
		UDP_Inst = m_internet->open_socket(UDP_port);
	} else {
		print_module_label();
		LPrintln("No internet platform, could not get UDP object");
	}
}

///////////////////////////////////////////////////////////////////////////////



