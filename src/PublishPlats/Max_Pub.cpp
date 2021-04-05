///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Max_Pub.cpp
/// @brief		File for MaxPub implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#if defined(LOOM_INCLUDE_MAX) && (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET))

#include "Max_Pub.h"
#include "../SubscribePlats/Max_Sub.h"
#include "../Manager.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////

#define UDP_SEND_OFFSET 8000 ///< UDP sending port is this value + device instance number

///////////////////////////////////////////////////////////////////////////////
MaxPub::MaxPub()
	: PublishPlat("MaxPub")
	, remoteIP({192,168,1,255})
{}

///////////////////////////////////////////////////////////////////////////////
void MaxPub::second_stage_ctor()
{
	PublishPlat::second_stage_ctor();
  LMark;

	UDP_port = UDP_SEND_OFFSET + ((device_manager) ? device_manager->get_instance_num() : 0);

	// Get new UDP pointer
	if (m_internet != nullptr) {
   	LMark;
		UDP_Inst = m_internet->open_socket(UDP_port);
	} else {
		print_module_label();
		LPrintln("No internet platform, could not get UDP object");
	}
}

///////////////////////////////////////////////////////////////////////////////
MaxPub::MaxPub(JsonArrayConst p)
	: MaxPub() {}

///////////////////////////////////////////////////////////////////////////////
void MaxPub::print_config() const
{
	PublishPlat::print_config();
	LPrintln("\tUDP Port  : ", UDP_port);
	LPrintln("\tRemote IP : ", remoteIP);
}

///////////////////////////////////////////////////////////////////////////////
void MaxPub::set_port(const uint16_t port)
{
	UDP_port = port;
  LMark;
	UDP_Inst = (m_internet != nullptr) ? m_internet->open_socket(UDP_port) : nullptr;
}

///////////////////////////////////////////////////////////////////////////////
bool MaxPub::send_to_internet(const JsonObject json, InternetPlat* plat)
{
	if (!UDP_Inst) {
   	LMark;
		// Try to get UDP
		UDP_Inst = (m_internet != nullptr) ? m_internet->open_socket(UDP_port) : nullptr;
		// Check if still null
		if (!UDP_Inst) {
			print_module_label();
			LPrintln("UDP_Inst is null");
			return false;
		}
	}

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("Sending to remoteIP: ", remoteIP);
	}

	// Send Json
	UDP_Inst->beginPacket(remoteIP, UDP_port);
  LMark;
	serializeJson(json, (*UDP_Inst) );
	UDP_Inst->endPacket(); // Mark the end of the OSC Packet
}

///////////////////////////////////////////////////////////////////////////////
void MaxPub::set_ip(const IPAddress ip)
{
	remoteIP = ip;
	print_module_label();
	LPrintln("Set remote IP to: ", ip);
}

///////////////////////////////////////////////////////////////////////////////
void MaxPub::set_ip()
{
	print_module_label();
	LPrintln("Received command to set IP to send to");
	MaxSub* temp;
	if (device_manager && ( temp = (device_manager->get<MaxSub>())) ) {
   	LMark;
		IPAddress ip = temp->get_remote_IP();
		if (ip[0] != 0) {
    	LMark;
			set_ip(ip);
		} else {
			print_module_label();
			LPrintln("IP was invalid");
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
bool MaxPub::dispatch(JsonObject json)
{
	JsonArray params = json["params"];
  LMark;
	switch( (char)json["func"] ) {
		case 's': set_ip(); return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////

#endif // if defined(LOOM_INCLUDE_MAX) && (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET))
