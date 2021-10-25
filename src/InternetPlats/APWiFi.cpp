///////////////////////////////////////////////////////////////////////////////
///
/// @file		APWiFi.cpp
/// @brief		File for APWiFi implementation.
/// @author		Luke Goertzen
/// @date		2021
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "APWiFi.h"
#include "../Manager.h"

///////////////////////////////////////////////////////////////////////////////
Loom_APWiFi::Loom_APWiFi(
		LoomManager*	manager
	)
	: LoomInternetPlat(manager, "APWiFi", Type::APWiFi )
	, password{""}
	, server{80}
{
	// Configure pins for Adafruit ATWINC1500 Feather
	WiFi.setPins(8,7,4,2);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_APWiFi::second_stage_ctor()
{
	// Set SSID (get info from manager if available)
	if (device_manager) {
		char tmpBuf[20];
		device_manager->get_device_name(tmpBuf);
		SSID = String(tmpBuf) + String(device_manager->get_instance_num());
	} else {
		SSID = String("Feather");
	}

	// Check for the presence of the shield, else disable WiFi module
	if (WiFi.status() == WL_NO_SHIELD) {
		print_module_label();
		LPrintln("WiFi shield not present");
		return;
	}

	start_AP(); // maybe put this in a second stage constructor
}

///////////////////////////////////////////////////////////////////////////////
Loom_APWiFi::Loom_APWiFi(LoomManager* manager, JsonArrayConst p)
	: Loom_APWiFi(manager) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_APWiFi::add_config(JsonObject json)
{
	JsonArray params = add_config_temp(json, module_name);
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_APWiFi::start_AP()
{
	print_module_label();
	LPrintln("Starting access point");

	auto status = WiFi.beginAP(SSID.c_str());

	if (status != WL_AP_LISTENING) {
		LPrintln("Creating access point failed");
		return false;
	}

	delay(10000);
	server.begin();

	print_state();

	return true;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_APWiFi::disconnect()
{
	WiFi.disconnect();
	WiFi.end();
	delay(200);
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_APWiFi::is_connected() const
{
	return WiFi.status() == WL_CONNECTED; // WL_AP_LISTENING
}

///////////////////////////////////////////////////////////////////////////////
LoomInternetPlat::UDPPtr Loom_APWiFi::open_socket(const uint port)
{
	// create the unique pointer
	LoomInternetPlat::UDPPtr ptr = LoomInternetPlat::UDPPtr(new WiFiUDP());
	// use the object created to open a UDP socket
	if (ptr && ptr->begin(port)) return std::move(ptr);
	// return a nullptr if any of that failed
	return LoomInternetPlat::UDPPtr();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_APWiFi::print_config() const
{
	LoomInternetPlat::print_config();
	LPrint("\tSSID:               : ", SSID, '\n');
}

///////////////////////////////////////////////////////////////////////////////
void Loom_APWiFi::print_state() const
{
	LoomInternetPlat::print_state();
	const char *text = m_wifi_status_to_string(WiFi.status());
	if (text != nullptr) {
		LPrintln("\tWireless state      :", text);
	} else {
		LPrintln("\tWireless state      :", WiFi.status());
	}
	LPrintln("\tConnected:          : ", (is_connected()) ? "True" : "False");
	LPrintln("\tSSID:               : ", SSID);
	LPrintln("\tRSSi:               : ", WiFi.RSSI(), " dBm");
	LPrintln("\tIP Address:         : ", IPAddress(WiFi.localIP()));
	LPrintln();
}

///////////////////////////////////////////////////////////////////////////////
const char* Loom_APWiFi::m_wifi_status_to_string(const uint8_t status) {
	switch (status) {
		case WL_NO_SHIELD: return "NO_SHIELD";
		case WL_IDLE_STATUS: return "IDLE_STATUS";
		case WL_NO_SSID_AVAIL: return "NO_SSID_AVAIL";
		case WL_SCAN_COMPLETED: return "SCAN_COMPLETED";
		case WL_CONNECTED: return "CONNECTED";
		case WL_CONNECT_FAILED: return "CONNECT_FAILED";
		case WL_CONNECTION_LOST: return "CONNECTION_LOST";
		case WL_DISCONNECTED: return "DISCONNECTED";
		// AP states (WL_AP_* and WL_PROVISIONING_*) are ignored for now
		default: return nullptr;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_APWiFi::package(JsonObject json)
{
	auto ip = IPAddress(WiFi.localIP());
	JsonArray tmp = json["id"].createNestedArray("ip");
	tmp.add(ip[0]);
	tmp.add(ip[1]);
	tmp.add(ip[2]);
	tmp.add(ip[3]);
}

///////////////////////////////////////////////////////////////////////////////
IPAddress Loom_APWiFi::get_ip()
{
	return IPAddress(WiFi.localIP());
}

///////////////////////////////////////////////////////////////////////////////
