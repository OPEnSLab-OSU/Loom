
#include "Loom_InternetWiFi.h"


/////////////////////////////////////////////////////////////////////
char* Loom_WiFi_I::enum_wifi_mode_string(WiFiMode c)
{
	switch(c) {
		case WiFiMode::AP 			: return "AP";
		case WiFiMode::WPA_CLIENT 	: return "WPA_CLIENT";
		// case WEP_CLIENT_MODE : return "WEP_CLIENT_MODE";
	}
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_WiFi_I::Loom_WiFi_I(	
		char*		module_name,

		WiFiMode	mode,
		char*		SSID,
		char*		pass,
		uint		UDP_port
	) 
	: LoomInternetPlat( module_name )
{
	// Configure pins for Adafruit ATWINC1500 Feather
	WiFi.setPins(8,7,4,2);      
		
	// Check for the presence of the shield, else disable WiFi module
	if (WiFi.status() == WL_NO_SHIELD) {
		Println("WiFi shield not present, entering infinite loop");
		active = false; 
		return;
	}

	this->mode 		= mode;
	this->UDP_port 	= UDP_port;
	strcpy(this->SSID, SSID);
	strcpy(this->pass, pass);

	server = new WiFiServer(80);

	connect();
}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_WiFi_I::~Loom_WiFi_I()
{
	delete server;
	// Check for the presence of the shield, else don't continue:
	if (WiFi.status() == WL_NO_SHIELD) {
		Println("WiFi shield not present, entering infinite loop");
		while (true); 
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_WiFi_I::print_config()
{
	if (!active) return;

	LoomInternetPlat::print_config();
	Println3('\t', "Mode:               : ", enum_wifi_mode_string(mode) );
}

/////////////////////////////////////////////////////////////////////
void Loom_WiFi_I::print_state()
{
	if (!active) return;

	LoomInternetPlat::print_state();	

	Println3('\t', "Mode:               : ", enum_wifi_mode_string(mode) );
	
	switch (mode) {
		case WiFiMode::AP :
			// Println3('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
			// Println3('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
			Print2('\t', "Connected MAC Addr  : " );
			status = WiFi.status();
			if (status == WL_AP_CONNECTED) { 	// A computer has connected to the AP
				byte remoteMac[6];
				WiFi.APClientMacAddress(remoteMac);
				print_MAC(remoteMac);
			} else {							// A computer has disconnected from the AP, and we are back in listening mode 
				Println("None");
			}
			// print_remote_AP_device_status();

			break;

		case WiFiMode::WPA_CLIENT :
			// Println3('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
			Println3('\t', "SSID:               : ", SSID );

			// Println3('\t', "IP Address:         : ", WiFi.localIP() );
			// Print2('\t', "MAC Address:        : " ); print_MAC();
			// Println4('\t', "RSSi:               : ", WiFi.RSSI(), " dBm" );
			// Println3('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
			// Println3('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
			break;
	}
	Println3('\t', "IP Address:         : ", WiFi.localIP() );
	Print2(  '\t', "MAC Address:        : " ); print_MAC(mac);
	Println4('\t', "RSSi:               : ", WiFi.RSSI(), " dBm" );
	Println();
}

/////////////////////////////////////////////////////////////////////
bool Loom_WiFi_I::connect()
{
	switch (mode) {
		case WiFiMode::AP         : return connect_AP();
		case WiFiMode::WPA_CLIENT : return connect_WPA();
		default              : return false;
	}
}

/////////////////////////////////////////////////////////////////////
bool Loom_WiFi_I::is_connected()
{
	if (!active) return false;



}

/////////////////////////////////////////////////////////////////////
bool Loom_WiFi_I::connect_AP()
{
	if (!active) return false;

	mode = WiFiMode::AP;

	// Print the AP network name (SSID);
	print_module_label();
	char AP_name[20];
	strcpy(AP_name, (device_manager) ? device_manager->get_device_name() : "UnknownFeather" );
	Println2("Creating access point named: ", AP_name );

	// Create open network
	status = WiFi.beginAP(AP_name);
	if (status != WL_AP_LISTENING) {
		print_module_label();
		Println("Create access point failed");
		active = false;
		return false;
	}

	// Wait 10 seconds for connection:
	delay(10000);     		
	
	// Start the web server on port 80, and start UDP 
	server->begin();   		
	UDP.begin(UDP_port);

	// Complete
	print_state();
	print_module_label();
	Println("Start AP done");

	if (device_manager) {
		device_manager->flash_LED(LED_WiFi_connected);
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
// Omitting SSID and password will try to connect in saved network
//   If they are provided but cannot connect, will revert to last mode,
//   Falling back to AP if all else fails
bool Loom_WiFi_I::connect_WPA(char* new_SSID, char* new_pass)
{
	if (!active) return false;

	status;

	// Try to connect, attempting the connection up to 10 times (this number is arbitrary)
	int attempt_count = 0;
	do {
		// print_module_label();
		// Println("Connecting to WPA host failed, trying again");

		// Try new network if provided
		print_module_label();
		if ( strlen(new_SSID) ) {
			Println2("Trying to connect to : ", new_SSID);
			status = WiFi.begin(new_SSID, new_pass);
			attempt_count++;
		} else {
			Println2("Trying to connect to : ", SSID);
			status = WiFi.begin(SSID, pass);
			attempt_count++;
		}

	} while (status != WL_CONNECTED && attempt_count < 5);


	// If not successfully connected try previous network or AP
	if (status != WL_CONNECTED) {
		print_module_label();
		Println("Connecting to WPA host failed completely");
		print_module_label();
		Println("Reverting to previous WiFi network, else AP mode");

		UDP.stop();
		WiFi.disconnect();
		WiFi.end();

		// If previous mode was WPA_CLIENT try last network
		if ( (mode == WiFiMode::WPA_CLIENT) && (strlen(new_SSID) == 0) ) {
			print_module_label();
			Println("Try to revert to previous WPA network");
			status = WiFi.begin(SSID, pass);
		}

		// If previous mode was not WPA or previous WPA failed, revert to AP
		if (status != WL_CONNECTED) {
			UDP.stop();
			WiFi.disconnect();
			WiFi.end();			
			print_module_label();
			Println("Reverting to AP");
			connect_AP();
			return false;
		}
	}
	
	// Restart server and UDP
	delay(8000); 
	server->begin();
	UDP.begin(UDP_port);

	print_state();

	if (device_manager) {
		device_manager->flash_LED(LED_WiFi_connected);
	}

	mode = WiFiMode::WPA_CLIENT;

	return true;
}

/////////////////////////////////////////////////////////////////////
uint32_t Loom_WiFi_I::get_time()
{
	if (!active) return 0;

	return WiFi.getTime();
}

/////////////////////////////////////////////////////////////////////
void Loom_WiFi_I::print_MAC(byte mac[])
{
	for (int i = 0; i < 5; i++) {
		Print_Hex(mac[0]);
		Print(":");		
	}
	Println(mac[5]);

}

// void Loom_WiFi_I::print_remote_AP_device_status()
// {
// 	// // Compare the previous status to the current status
// 	// if (status != WiFi.status()) {
// 	// 	status = WiFi.status();				// It has changed, update the variable
		
// 	if (status == WL_AP_CONNECTED) { 	// A computer has connected to the AP
// 		byte remoteMac[6];
// 		WiFi.APClientMacAddress(remoteMac);
// 		Print2('\t', "Connected MAC Addr  : " ); print_MAC(remoteMac);
// 	} else {							// A computer has disconnected from the AP, and we are back in listening mode 
// 		Println("No device connected connected to AP");
// 	}
// 	// }
	// Print the network name (SSID);
	// Println2("Creating access point named: ", config_wifi->my_ssid);

	// // Create open network. Change this line if you want to create an WEP network:
	// status = WiFi.beginAP(config_wifi->my_ssid);
	// if (status != WL_AP_LISTENING) {
	// 	Println("Creating access point failed");
	// 	while (true);   	// Don't continue
	// }

	// delay(10000);     		// Wait 10 seconds for connection:
	
	// server.begin();   		// Start the web server on port 80

	// printWiFiStatus();   	// You're connected now, so print out the status
	// Println("\nStarting UDP connection over server...");
		
	// // If you get a connection, report back via serial:
	// UdpDevice.begin(config_wifi->devicePort);

	// Println("Done");
	// flash_led(6, 50, 50);



