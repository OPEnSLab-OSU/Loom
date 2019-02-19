
#include "Loom_InternetWiFi.h"



char* Loom_WiFi_I::enum_wifi_mode_string(WiFiMode c)
{
	switch(c) {
		case AP_MODE         : return "AP_MODE";
		case WPA_CLIENT_MODE : return "WPA_CLIENT_MODE";
		case WEP_CLIENT_MODE : return "WEP_CLIENT_MODE";
	}
}


// --- CONSTRUCTOR ---
Loom_WiFi_I::Loom_WiFi_I(	char* module_name,

							char* ssid,
							char* pass

				) : LoomInternetPlat( module_name )
{
	// Configure pins for Adafruit ATWINC1500 Feather
	WiFi.setPins(8,7,4,2);      
		
	// Check for the presence of the shield, else don't continue:
	if (WiFi.status() == WL_NO_SHIELD) {
		Println("WiFi shield not present, entering infinite loop");
		while (true); 
	}
}

	// --- DESTRUCTOR ---
Loom_WiFi_I::~Loom_WiFi_I()
{

}


void Loom_WiFi_I::print_config()
{
	LoomInternetPlat::print_config();	
}


void Loom_WiFi_I::print_state()
{
	LoomInternetPlat::print_state();	
	// Println3('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
}




bool Loom_WiFi_I::connect_AP()
{
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
}


bool Loom_WiFi_I::connect_WPA()
{

}



// bool Loom_WiFi_I::log_to_pushingbox(OSCBundle* bndl)
// {
	
// }
