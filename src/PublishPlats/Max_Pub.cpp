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
  LMark;
	PublishPlat::second_stage_ctor();
  LMark;

	UDP_port = UDP_SEND_OFFSET + ((device_manager) ? device_manager->get_instance_num() : 0);
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
MaxPub::MaxPub(JsonArrayConst p)
	: MaxPub() {}

///////////////////////////////////////////////////////////////////////////////
void MaxPub::print_config() const
{
  LMark;
	PublishPlat::print_config();
  LMark;
	LPrintln("\tUDP Port  : ", UDP_port);
  LMark;
	LPrintln("\tRemote IP : ", remoteIP);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void MaxPub::set_port(const uint16_t port)
{
  LMark;
	UDP_port = port;
  LMark;
	UDP_Inst = (m_internet != nullptr) ? m_internet->open_socket(UDP_port) : nullptr;
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
bool MaxPub::send_to_internet(const JsonObject json, InternetPlat* plat)
{
  LMark;
	if (!UDP_Inst) {
   	LMark;
		// Try to get UDP
		UDP_Inst = (m_internet != nullptr) ? m_internet->open_socket(UDP_port) : nullptr;
   	LMark;
		// Check if still null
		if (!UDP_Inst) {
    	LMark;
			print_module_label();
    	LMark;
			LPrintln("UDP_Inst is null");
    	LMark;
			return false;
   LMark;
		}
	}

	if (print_verbosity == Verbosity::V_HIGH) {
   	LMark;
		print_module_label();
   	LMark;
		LPrintln("Sending to remoteIP: ", remoteIP);
  	LMark;
	}

	// Send Json
	UDP_Inst->beginPacket(remoteIP, UDP_port);
  LMark;
	serializeJson(json, (*UDP_Inst) );
  LMark;
	UDP_Inst->endPacket(); // Mark the end of the OSC Packet
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void MaxPub::set_ip(const IPAddress ip)
{
  LMark;
	remoteIP = ip;
  LMark;
	print_module_label();
  LMark;
	LPrintln("Set remote IP to: ", ip);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void MaxPub::set_ip()
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Received command to set IP to send to");
  LMark;
	MaxSub* temp;
  LMark;
	if (device_manager && ( temp = (device_manager->get<MaxSub>())) ) {
   	LMark;
		IPAddress ip = temp->get_remote_IP();
   	LMark;
		if (ip[0] != 0) {
    	LMark;
			set_ip(ip);
   		LMark;
		} else {
			print_module_label();
    	LMark;
			LPrintln("IP was invalid");
   		LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
bool MaxPub::dispatch(JsonObject json)
{
  LMark;
	JsonArray params = json["params"];
  LMark;
	switch( (char)json["func"] ) {
   LMark;
		case 's': set_ip(); return true;
  LMark;
	}
	return false;
 LMark;
}

///////////////////////////////////////////////////////////////////////////////

#endif // if defined(LOOM_INCLUDE_MAX) && (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET))
