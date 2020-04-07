///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_InternetWiFi.cpp
/// @brief		File for Loom_WiFi implementation.
/// @author		Noah Koontz
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "InternetWiFi.h"

///////////////////////////////////////////////////////////////////////////////
Loom_WiFi::Loom_WiFi(	
		LoomManager* manager,
		const char* 	ssid,
		const char* 	pass
	) 
	: LoomInternetPlat(manager, "WiFi", Type::WiFi )
	, SSID(ssid)
	, pass(pass)
	, m_base_client()
	, m_client(m_base_client, TAs, (size_t)TAs_NUM, A7, 1, SSLClient::SSL_INFO)
{
	// Configure pins for Adafruit ATWINC1500 Feather
	WiFi.setPins(8,7,4,2);      
		
	// Check for the presence of the shield, else disable WiFi module
	if (WiFi.status() == WL_NO_SHIELD) {
		print_module_label();
		LPrintln("WiFi shield not present");
		return;
	}

	connect();
}

///////////////////////////////////////////////////////////////////////////////
Loom_WiFi::Loom_WiFi(LoomManager* manager, JsonArrayConst p)
	: Loom_WiFi(manager, EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_WiFi::connect()
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
			status = WiFi.begin(SSID);			
		} else {
			status = WiFi.begin(SSID, pass);
		}
		attempt_count++;

		// debug print!
		if (last_status != status) {
			print_module_label();
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
void Loom_WiFi::disconnect() {
	// tell the wifi it's time to stop
	WiFi.disconnect();
	delay(200);
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_WiFi::is_connected() const
{
	return WiFi.status() == WL_CONNECTED;
}

///////////////////////////////////////////////////////////////////////////////
LoomInternetPlat::UDPPtr Loom_WiFi::open_socket(const uint port) 
{
	// create the unique pointer
	UDPPtr ptr = UDPPtr(new WiFiUDP());
	// use the object created to open a UDP socket
	if (ptr && ptr->begin(port)) return std::move(ptr);
	// return a nullptr if any of that failed
	return UDPPtr();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_WiFi::print_config() const
{
	LoomInternetPlat::print_config();
	LPrint("\tSSID:               : ", SSID, '\n');
}

///////////////////////////////////////////////////////////////////////////////
void Loom_WiFi::print_state() const
{
	LoomInternetPlat::print_state();
	const char* text = m_wifi_status_to_string(WiFi.status());
	if (text != nullptr)
		LPrintln("\tWireless state      :", text );	
	else
	LPrintln("\tWireless state      :", WiFi.status() );	
	LPrintln("\tConnected:          : ", (is_connected()) ? "True" : "False" );
	LPrintln("\tSSID:               : ", SSID );
	LPrintln("\tRSSi:               : ", WiFi.RSSI(), " dBm" );
	LPrintln("\tIP Address:         : ", IPAddress(WiFi.localIP()) );
	LPrintln();
}

///////////////////////////////////////////////////////////////////////////////
const char* Loom_WiFi::m_wifi_status_to_string(const uint8_t status) {
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
