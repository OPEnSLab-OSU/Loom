
#include "Loom_InternetWiFi.h"

///////////////////////////////////////////////////////////////////////////////
Loom_WiFi::Loom_WiFi(	
		const char* 	ssid,
		const char* 	pass
	) 
	: LoomInternetPlat( "WiFi", Type::WiFi )
	, SSID(ssid)
	, pass(pass)
	, client()
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
Loom_WiFi::Loom_WiFi(JsonArrayConst p)
	: Loom_WiFi( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_WiFi::connect()
{
	// Try to connect, attempting the connection up to 5 times (this number is arbitrary)
	uint8_t attempt_count = 0;
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
bool Loom_WiFi::is_connected()
{
	return WiFi.status() == WL_CONNECTED;
}

///////////////////////////////////////////////////////////////////////////////
LoomInternetPlat::ClientSession Loom_WiFi::connect_to_domain(const char* domain)
{
	// if the socket is somehow still open, close it
	if (client.connected()) client.stop();
	// * the rainbow connection *
	int status = client.connect(domain, 443);
	if (!status) {
		// log fail, and return
		print_module_label();
		LPrint("Wifi connect failed with error ", status, '\n');
		return ClientSession();
	}
	else {
		print_module_label();
		LPrint("WiFi connected to domain: ", domain, '\n');
	}
	// return a pointer to the client for data reception
	return LoomInternetPlat::ClientSession(&client);
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
void Loom_WiFi::print_config()
{
	LoomInternetPlat::print_config();
	LPrint("\tSSID:               : ", SSID, '\n');
}

///////////////////////////////////////////////////////////////////////////////
void Loom_WiFi::print_state()
{
	LoomInternetPlat::print_state();	
	LPrintln("\tConnected:          : ", (is_connected()) ? "True" : "False" );
	LPrintln("\tSSID:               : ", SSID );
	LPrintln("\tRSSi:               : ", WiFi.RSSI(), " dBm" );
	LPrintln("\tIP Address:         : ", IPAddress(WiFi.localIP()) );
	LPrintln();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_WiFi::package(JsonObject json)
{
	//JsonObject data = get_module_data_object(json, module_name);
	auto ip = IPAddress(WiFi.localIP());
	JsonArray tmp = json["id"].createNestedArray("ip");
	tmp.add(ip[0]);
	tmp.add(ip[1]);
	tmp.add(ip[2]);
	tmp.add(ip[3]);
	//data["IP"] = WiFi.localIP();
}

///////////////////////////////////////////////////////////////////////////////
