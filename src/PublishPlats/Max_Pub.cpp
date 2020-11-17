///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Max_Pub.cpp
/// @brief		File for Loom_MaxPub implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "Max_Pub.h"
#include "../SubscribePlats/Max_Sub.h"
#include "../Manager.h"


///////////////////////////////////////////////////////////////////////////////
Loom_MaxPub::Loom_MaxPub(
		LoomManager* manager,
		const LoomModule::Type	internet_type
	)
	: LoomPublishPlat(manager, "MaxPub", Type::MaxPub, internet_type )
	, remoteIP({192,168,1,255})
{}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxPub::second_stage_ctor()
{
  LMark;
	LoomPublishPlat::second_stage_ctor();
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
Loom_MaxPub::Loom_MaxPub(LoomManager* manager, JsonArrayConst p)
	: Loom_MaxPub(manager, (LoomModule::Type)(int)p[0] ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxPub::print_config() const
{
  LMark;
	LoomPublishPlat::print_config();
  LMark;
	LPrintln("\tUDP Port  : ", UDP_port);
  LMark;
	LPrintln("\tRemote IP : ", remoteIP);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxPub::set_port(const uint16_t port)
{
  LMark;
	UDP_port = port;
  LMark;
	UDP_Inst = (m_internet != nullptr) ? m_internet->open_socket(UDP_port) : nullptr;
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_MaxPub::send_to_internet(const JsonObject json, LoomInternetPlat* plat)
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
void Loom_MaxPub::set_ip(const IPAddress ip)
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
void Loom_MaxPub::set_ip()
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Received command to set IP to send to");
  LMark;
	Loom_MaxSub* temp;
  LMark;
	if (device_manager && (temp = (Loom_MaxSub*)&(device_manager->MaxSub())) ) {
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
bool Loom_MaxPub::dispatch(JsonObject json)
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
