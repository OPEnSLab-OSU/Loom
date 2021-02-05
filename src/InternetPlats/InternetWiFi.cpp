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
  LMark;

	// Check for the presence of the shield, else disable WiFi module
	if (::WiFi.status() == WL_NO_SHIELD) {
   	LMark;
		print_module_label();
   	LMark;
		LPrintln("WiFi shield not present");
   	LMark;
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
  LMark;
	// clear the write error
	m_base_client.clearWriteError();
  LMark;
	// Try to connect, attempting the connection up to 5 times (this number is arbitrary)
	uint8_t attempt_count = 0;
  LMark;
	uint8_t last_status = 0;
  LMark;
	uint8_t status = 0;
  LMark;
	do {
   	LMark;
		print_module_label();
   	LMark;
		LPrintln("Trying to connect to: ", SSID);
  	LMark;

		// Check if password provided
		if (pass == nullptr || pass[0] == '\0' ) {
    	LMark;
			status = ::WiFi.begin(SSID);
   		LMark;
		} else {
			status = ::WiFi.begin(SSID, pass);
   		LMark;
		}
		attempt_count++;
  	LMark;

		// debug print!
		if (last_status != status) {
    	LMark;
			print_module_label();
    	LMark;
			const char* text = m_wifi_status_to_string(status);
    	LMark;
			if (text != nullptr)
				LPrint("Status changed to: ", text, '\n');
			else
				LPrint("Status changed to: ", status, '\n');
   		LMark;
		}
		delay(2000);
  	LMark;
	} while (status != WL_CONNECTED && attempt_count < 5);

	if (attempt_count == 5) {
   	LMark;
		print_module_label();
   	LMark;
		LPrintln("Connection failed!");
   	LMark;
		return;
	}
	print_state();
}

///////////////////////////////////////////////////////////////////////////////
void WiFi::disconnect() {
  LMark;
	// tell the wifi it's time to stop
	::WiFi.disconnect();
  LMark;
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
  LMark;
	// use the object created to open a UDP socket
	if (ptr && ptr->begin(port)) return std::move(ptr);
  LMark;
	// return a nullptr if any of that failed
	return UDPPtr();
}

///////////////////////////////////////////////////////////////////////////////
void WiFi::print_config() const
{
  LMark;
	InternetPlat::print_config();
  LMark;
	LPrint("\tSSID:               : ", SSID, '\n');
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void WiFi::print_state() const
{
  LMark;
	InternetPlat::print_state();
  LMark;
	const char* text = m_wifi_status_to_string(::WiFi.status());
  LMark;
	if (text != nullptr)
		LPrintln("\tWireless state      :", text );
	else
		LPrintln("\tWireless state      :", ::WiFi.status() );
  LMark;
	LPrintln("\tConnected:          : ", (is_connected()) ? "True" : "False" );
  LMark;
	LPrintln("\tSSID:               : ", SSID );
  LMark;
	LPrintln("\tRSSi:               : ", ::WiFi.RSSI(), " dBm" );
  LMark;
	LPrintln("\tIP Address:         : ", IPAddress(::WiFi.localIP()) );
  LMark;
	LPrintln();
 	LMark;
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
  LMark;
	tmp.add(ip[0]);
  LMark;
	tmp.add(ip[1]);
  LMark;
	tmp.add(ip[2]);
  LMark;
	tmp.add(ip[3]);
  LMark;
	//data["IP"] = ::WiFi.localIP();
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_MAX

#endif // ifdef LOOM_INCLUDE_WIFI
