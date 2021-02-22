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
#include "../InternetPlats/InternetWiFi.h"
#include "../InternetPlats/APWiFi.h"
#include "../PublishPlats/Max_Pub.h"
// #include "Module_Factory.h"

#include <WiFi101.h>
#include <WiFiUdp.h>


#include "../Manager.h"

///////////////////////////////////////////////////////////////////////////////

#define UDP_RECEIVE_OFFSET 9000 ///< UDP receiving port is this value + device instance number

///////////////////////////////////////////////////////////////////////////////
Loom_MaxSub::Loom_MaxSub(
		LoomManager* manager,
		const LoomModule::Type	internet_type,
		const bool				auto_dispatch
	)   
	: LoomSubscribePlat(manager, "MaxSub", Type::MaxSub, internet_type )
	, auto_dispatch(auto_dispatch)
	, wifi_mode{WiFiMode::INVALID}
{}

///////////////////////////////////////////////////////////////////////////////
Loom_MaxSub::Loom_MaxSub(LoomManager* manager, JsonArrayConst p) 
	: Loom_MaxSub(manager, (LoomModule::Type)(int)p[0], p[1] ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxSub::add_config(JsonObject json)
{
	JsonArray params = add_config_temp(json, module_name);
	params.add(auto_dispatch);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxSub::second_stage_ctor() 
{
	LoomSubscribePlat::second_stage_ctor();

	UDP_port = UDP_RECEIVE_OFFSET + ((device_manager) ? device_manager->get_instance_num() : 0);

	// Get new UDP pointer	
	if (m_internet) {
		get_wifi_mode(); // determine wifi mode
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

	//Not sure why this code being here made any difference...
    auto status = ::WiFi.status();


	if (wifi_mode == WiFiMode::AP) {
		print_module_label();
		// Maybe don't bother sending if nothing is connected?
		// Would only apply to AP mode though
		if (status == WL_AP_CONNECTED) {
		byte remoteMac[6];

		// a device has connected to the AP
		Serial.print("Device connected to AP, MAC address: ");
		::WiFi.APClientMacAddress(remoteMac);
		LPrint_Hex(remoteMac[5]);
		LPrint_Hex(remoteMac[4]);
		LPrint_Hex(remoteMac[3]);
		LPrint_Hex(remoteMac[2]);
		LPrint_Hex(remoteMac[1]);
		LPrint_Hex(remoteMac[0]);
		LPrintln();
		} else {
			Serial.println("No device connected to AP");
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
	
	return false;
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
bool MaxSub::dispatch(JsonObject json)
{
	JsonArray params = json["params"];
	switch( (char)json["func"] ) {
		case 'a': return goto_ap_mode();
		case 'c': if (params.size() >= 2) { goto_client_mode(EXPAND_ARRAY(params, 2)); LPrintln("Exitted"); } return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_MaxSub::goto_ap_mode()
{
	// Make sure WiFi mode is up to date
	get_wifi_mode(); 

	bool success = true;
	InternetPlat* previous_wifi = m_internet;

	switch (wifi_mode) {
		case WiFiMode::AP: // Already in AP mode
			print_module_label();
			LPrintln("Already in AP mode");
			return true;
		
		case WiFiMode::CLIENT:
		{
			print_module_label();
			LPrintln("Switching to AP mode");

			// Disconnect current WiFi 
			previous_wifi->disconnect();

			// Instantiate APWiFi module
			m_internet = new APWiFi();

			// Confirm connection 
			// if (! m_internet->is_connected()) {
			// 	print_module_label();
			// 	LPrintln("Failed to start AP");
			// 	success = false;
			// 	break;
			// }

			// Pass WiFi module to manager
			if (device_manager) {
				device_manager->add_module(m_internet);
			} else {
				success = false;
				break;
			}

			m_internet->second_stage_ctor();

			// Successfully switched, remove WiFI module and switch WiFi mode
			device_manager->remove_module<WiFi>();
			wifi_mode = WiFiMode::CLIENT;
			break;
		}

		default: // Invalid WiFi mode, cannot switch mode
			return false;
	}

	if (!success) {
		print_module_label();
		LPrintln("Failed to swtich to AP mode");
		delete m_internet;
		m_internet = previous_wifi;
		m_internet->connect();
	}

	// Reopen MaxPub socket 
	if (device_manager) {
		Module* tmp = device_manager->get_by_name("MaxPub");
		if (tmp) {
			((MaxPub*)tmp)->set_internet_plat(m_internet);
		}
	}

	UDP_Inst = m_internet->open_socket(UDP_port);
	
	return success;

}

///////////////////////////////////////////////////////////////////////////////
bool Loom_MaxSub::goto_client_mode(const char* ssid, const char* pass)
{
	// Make sure WiFi mode is up to date
	get_wifi_mode(); 

	bool success = true;
	InternetPlat* previous_wifi = m_internet;

	switch (wifi_mode) {
		case WiFiMode::AP: 
		{
			print_module_label();
			LPrintln("Switching to client mode");

			// Disconnect current WiFi 
			previous_wifi->disconnect(); 

			// Instantiate WiFi module
			m_internet = new Loom::WiFi(ssid, pass);

			// Test connection
			if (! m_internet->is_connected()) {
				print_module_label();
				LPrintln("Failed to connect");
				success = false;
				break;
			}

			// Pass WiFi module to manager
			if (device_manager) {
				device_manager->add_module(m_internet);
			} else {
				success = false;
				break;
			}

			// Successfully switched, remove AP WiFI module and switch WiFi mode
			device_manager->remove_module<APWiFi>();
			wifi_mode = WiFiMode::CLIENT;
			break;
		}

		case WiFiMode::CLIENT:
		{
			print_module_label();
			LPrintln("Connecting to new WiFi network");

			// Disconnect current WiFi 
			previous_wifi->disconnect(); 

			// Instantiate WiFi module
			m_internet = new Loom::WiFi(ssid, pass);

			// Test connection
			if (! m_internet->is_connected()) {
				print_module_label();
				LPrintln("Failed to connect");
				success = false;
				break;
			}

			// Successfully switched, remove AP WiFI module, switch WiFi mode, 
			// and pass new WiFi module to manager
			if (device_manager) {
				device_manager->remove_module<WiFi>();
				device_manager->add_module(m_internet);
			} else {
				success = false;
				break;
			}

			wifi_mode = WiFiMode::CLIENT;
			break;
		}

		default: // Invalid WiFi mode, cannot switch mode
			return false;
	}

	if (!success) {
		print_module_label();
		LPrintln("Failed to swtich to client mode");
		delete m_internet;
		m_internet = previous_wifi;
		m_internet->connect();
	}

	// Reopen MaxPub socket 
	if (device_manager) {
		Module* tmp = device_manager->get_by_name("MaxPub");
		if (tmp) {
			((MaxPub*)tmp)->set_internet_plat(m_internet);
		}
	}

	UDP_Inst = m_internet->open_socket(UDP_port);
	
	return success;
}

///////////////////////////////////////////////////////////////////////////////
Loom_MaxSub::WiFiMode Loom_MaxSub::get_wifi_mode() 
{
	wifi_mode = WiFiMode::INVALID;

	if (m_internet) {
		Module* tmp;
		if (tmp = device_manager->get_by_name("APWiFi")) {
			wifi_mode = WiFiMode::AP;
		} else if (tmp = device_manager->get_by_name("WiFi")) {	
			wifi_mode = WiFiMode::CLIENT;
		}
	}

	return wifi_mode;
}

///////////////////////////////////////////////////////////////////////////////



