///////////////////////////////////////////////////////////////////////////////
///
/// @file		Max_Pub.cpp
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
#include "../InternetPlats/InternetWiFi.h"
#include "../InternetPlats/APWiFi.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////

#define UDP_SEND_OFFSET 8000 ///< UDP sending port is this value + device instance number

///////////////////////////////////////////////////////////////////////////////
MaxPub::MaxPub()   
	: PublishPlat("MaxPub")
	// , remoteIP({192,168,1,255})
	, remoteIP({10,0,0,255})
{}

///////////////////////////////////////////////////////////////////////////////
void MaxPub::second_stage_ctor() 
{
	PublishPlat::second_stage_ctor();

	UDP_port = UDP_SEND_OFFSET + ((device_manager) ? device_manager->get_instance_num() : 0);

	// Get new UDP pointer	
	if (m_internet) {
		UDP_Inst = m_internet->open_socket(UDP_port);
		update_remote_ip();
	} else {
		print_module_label();
		LPrintln("No internet platform, could not get UDP object");
	}
}

///////////////////////////////////////////////////////////////////////////////
MaxPub::MaxPub(JsonArrayConst p)
	: MaxPub() {}

///////////////////////////////////////////////////////////////////////////////
void MaxPub::add_config(JsonObject json)
{
	JsonArray params = add_config_temp(json, module_name);
}

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
	UDP_Inst = (m_internet != nullptr) ? m_internet->open_socket(UDP_port) : nullptr;
}

///////////////////////////////////////////////////////////////////////////////
bool MaxPub::send_to_internet(const JsonObject json, InternetPlat* plat)
{
	if (!UDP_Inst) {
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
	serializeJson(json, (*UDP_Inst) );
	UDP_Inst->endPacket(); // Mark the end of the OSC Packet
	return true;
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
	Module* tmp;
	if (device_manager && ( tmp = device_manager->get_by_name("MaxSub")) ) {
		IPAddress ip = ((MaxSub*)tmp)->get_remote_IP();
		if (ip[0] != 0) {
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
	switch( (char)json["func"] ) {
		case 's': set_ip(); return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void MaxPub::set_internet_plat(InternetPlat* plat)
{
	m_internet = plat;
	if (m_internet) {
		set_port(UDP_port);
		update_remote_ip();
	}
}

///////////////////////////////////////////////////////////////////////////////
bool MaxPub::update_remote_ip()
{
	if (!m_internet || !device_manager) return false;

	IPAddress ip;
	Module* tmp;
	if (tmp = device_manager->get_by_name("WiFi")) {
		ip = ((WiFi*)tmp)->get_ip();
	} else if (tmp = device_manager->get_by_name("APWiFi")) {	
		ip = ((APWiFi*)tmp)->get_ip();
	} else {
		return false;
	}

	remoteIP = IPAddress(ip[0], ip[1], ip[2], 255);
	print_module_label();
	LPrintln("Set destination IP to: ", remoteIP);
	return true;
}

///////////////////////////////////////////////////////////////////////////////

#endif // if defined(LOOM_INCLUDE_MAX) && (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET))
