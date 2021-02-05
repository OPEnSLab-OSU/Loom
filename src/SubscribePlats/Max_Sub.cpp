///////////////////////////////////////////////////////////////////////////////
///
/// @file		Max_Sub.cpp
/// @brief		File for MaxSub implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#if defined(LOOM_INCLUDE_MAX) && (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET))

#include "Max_Sub.h"
#include "../Manager.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////

#define UDP_RECEIVE_OFFSET 9000 ///< UDP receiving port is this value + device instance number

///////////////////////////////////////////////////////////////////////////////
MaxSub::MaxSub(
		const bool				auto_dispatch
	)
	: SubscribePlat("MaxSub")
	, auto_dispatch(auto_dispatch)
{}

///////////////////////////////////////////////////////////////////////////////
MaxSub::MaxSub(JsonArrayConst p)
	: MaxSub((const bool)p[0]) {}

///////////////////////////////////////////////////////////////////////////////
void MaxSub::second_stage_ctor()
{
  LMark;
	SubscribePlat::second_stage_ctor();
  LMark;

	UDP_port = UDP_RECEIVE_OFFSET + ((device_manager) ? device_manager->get_instance_num() : 0);
  LMark;

	// Get new UDP pointer
	if (m_internet != nullptr) {
   	LMark;
		UDP_Inst = m_internet->open_socket(UDP_port);
  	LMark;
	} else {
		print_module_label();
   	LMark;
		LPrintln("No internet platform, could not get UDP object");
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
bool MaxSub::subscribe(JsonObject json)
{
  LMark;
	// Make sure UDP object exists
	if (!UDP_Inst) {
   	LMark;
		// Try to get UDP
		UDP_Inst = (m_internet != nullptr) ? m_internet->open_socket(UDP_port) : nullptr;
   	LMark;
		// Check if still null
		if (!UDP_Inst) {
    	LMark;
			LPrintln("UDP_Inst is null");
    	LMark;
			return false;
		}
	}

	// If packet available
	if ( UDP_Inst->parsePacket() ) {
  	LMark;

		messageJson.clear();
   	LMark;
		if ( deserializeJson(messageJson, (*UDP_Inst) ) != DeserializationError::Ok ) {
    	LMark;
			print_module_label();
    	LMark;
			LPrintln("Failed to parse MsgPack");
    	LMark;
			return false;
		}

		bool status = json.set(messageJson.as<JsonObject>());
   	LMark;
		if (!status) {
    	LMark;
			LPrintln("Json set error");
    	LMark;
			return false;
		}

		if (print_verbosity == Verbosity::V_HIGH) {
    	LMark;
			print_module_label();
    	LMark;
			LPrint("From IP: ");
    	LMark;
			IPAddress remote = UDP_Inst->remoteIP();
    	LMark;
			for (auto i=0; i < 4; i++) {
     		LMark;
				LPrint(remote[i]);
     		LMark;
				if (i < 3) {
      		LMark;
					LPrint(".");
     			LMark;
				} else {
					LPrintln();
     			LMark;
				}
			}

			print_module_label();
    	LMark;
			LPrintln("Internal messageJson:");
    	LMark;
			serializeJsonPretty(messageJson, Serial);
    	LMark;
			LPrintln();
   		LMark;
		}

		// If auto dispatch, have device manager dispatch command
		if (auto_dispatch && device_manager != nullptr) {
    	LMark;
			device_manager->dispatch();
   		LMark;
		}

		return true;
	}
}

///////////////////////////////////////////////////////////////////////////////
void MaxSub::print_config() const
{
  LMark;
	SubscribePlat::print_config();
  LMark;
	LPrintln("\tUDP Port : ", UDP_port);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void MaxSub::set_port(const uint16_t port)
{
  LMark;
	UDP_port = port;
  LMark;

	if (m_internet != nullptr) {
   	LMark;
		UDP_Inst = m_internet->open_socket(UDP_port);
  	LMark;
	} else {
		print_module_label();
   	LMark;
		LPrintln("No internet platform, could not get UDP object");
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////

#endif // if defined(LOOM_INCLUDE_MAX) && (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET))
