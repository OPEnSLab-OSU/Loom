///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_InternetWiFi.cpp
/// @brief		File for InternetPlat implementation.
/// @author		Noah Koontz
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_WIFI

#include "InternetWiFi.h"
#include "Trust_Anchors.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
WiFi::WiFi(
		const char* 	ssid,
		const char* 	pass
	)
	: InternetPlat("WiFi")
	, SSID(ssid)
	, pass(pass)
	, m_base_client()
	, m_client(m_base_client, TAs, (size_t)TAs_NUM, A7, 1, SSLClient::SSL_INFO)
{
  LMark;
	// Configure pins for Adafruit ATWINC1500 Feather
	::WiFi.setPins(8,7,4,2);

	// Check for the presence of the shield, else disable WiFi module
	if (::WiFi.status() == WL_NO_SHIELD) {
		print_module_label();
		LPrintln("WiFi shield not present");
		return;
	}

	connect();
}

///////////////////////////////////////////////////////////////////////////////
WiFi::WiFi(JsonArrayConst p)
	: WiFi(EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void WiFi::connect()
{
	// clear the write error
	m_base_client.clearWriteError();
	// Try to connect, attempting the connection up to 5 times (this number is arbitrary)
	uint8_t attempt_count = 0;
	uint8_t last_status = 0;
	uint8_t status = 0;
	do {
		print_module_label();
		LPrintln("Trying to connect to: ", SSID);

		// Check if password provided
		if (pass == nullptr || pass[0] == '\0' ) {
    	LMark;
			status = ::WiFi.begin(SSID);
		} else {
			status = ::WiFi.begin(SSID, pass);
		}
		attempt_count++;

		// debug print!
		if (last_status != status) {
			print_module_label();
    	LMark;
			const char* text = m_wifi_status_to_string(status);
			if (text != nullptr)
				LPrint("Status changed to: ", text, '\n');
			else
				LPrint("Status changed to: ", status, '\n');
		}
		delay(2000);
	} while (status != WL_CONNECTED && attempt_count < 5);

	if (attempt_count == 5) {
		print_module_label();
		LPrintln("Connection failed!");
		return;
	}
	print_state();
}

///////////////////////////////////////////////////////////////////////////////
void WiFi::disconnect() {
	// tell the wifi it's time to stop
	::WiFi.disconnect();
	delay(200);
}

///////////////////////////////////////////////////////////////////////////////
bool WiFi::is_connected() const
{
  LMark;
	return ::WiFi.status() == WL_CONNECTED;
}

///////////////////////////////////////////////////////////////////////////////
InternetPlat::UDPPtr WiFi::open_socket(const uint port)
{
  LMark;
	// create the unique pointer
	UDPPtr ptr = UDPPtr(new WiFiUDP());
	// use the object created to open a UDP socket
	if (ptr && ptr->begin(port)) return std::move(ptr);
	// return a nullptr if any of that failed
	return UDPPtr();
}

///////////////////////////////////////////////////////////////////////////////
void WiFi::print_config() const
{
	InternetPlat::print_config();
	LPrint("\tSSID:               : ", SSID, '\n');
}

///////////////////////////////////////////////////////////////////////////////
void WiFi::print_state() const
{
	InternetPlat::print_state();
	const char* text = m_wifi_status_to_string(::WiFi.status());
  LMark;
	if (text != nullptr)
		LPrintln("\tWireless state      :", text );
	else
		LPrintln("\tWireless state      :", ::WiFi.status() );
	LPrintln("\tConnected:          : ", (is_connected()) ? "True" : "False" );
	LPrintln("\tSSID:               : ", SSID );
	LPrintln("\tRSSi:               : ", ::WiFi.RSSI(), " dBm" );
	LPrintln("\tIP Address:         : ", IPAddress(::WiFi.localIP()) );
	LPrintln();
}

///////////////////////////////////////////////////////////////////////////////
const char* WiFi::m_wifi_status_to_string(const uint8_t status) {
  LMark;
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
#ifdef LOOM_INCLUDE_MAX

void WiFi::package(JsonObject json)
{
  LMark;
	//JsonObject data = get_module_data_object(json, module_name);
	auto ip = IPAddress(::WiFi.localIP());
  LMark;
	JsonArray tmp = json["id"].createNestedArray("ip");
	tmp.add(ip[0]);
	tmp.add(ip[1]);
	tmp.add(ip[2]);
	tmp.add(ip[3]);
	//data["IP"] = ::WiFi.localIP();
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_MAX

#endif // ifdef LOOM_INCLUDE_WIFI
