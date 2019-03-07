
#ifndef LOOM_INTERNET_WIFI_PLAT_h
#define LOOM_INTERNET_WIFI_PLAT_h

#include "Loom_InternetPlat.h"

#include <WiFi101.h>
#include <WiFiUdp.h>


enum class WiFiMode {
	AP,
	WPA_CLIENT
	// WEP_CLIENT_MODE
};



class Loom_WiFi_I : public LoomInternetPlat
{

protected:
	
	IPAddress		ip;                     // Device's IP Address

	char			SSID[32];               // Host network name
	char			pass[32];               // Host network password
	
	// char*			ip_broadcast;           // IP to Broadcast data
	// unsigned int 	devicePort;            // Local port to listen device specific messages on
	// unsigned int 	subnetPort; 			// Local port to listen for family subnet messages on	
	uint 			UDP_port;


	byte			mac[6];                 // Device's MAC Address
	WiFiMode 		mode;              // Devices current wifi mode
	
	WiFiServer*		server;
	WiFiUDP			UDP;

	uint8_t			status;


	// bool 		request_settings;       // True if device should request new channel settings on startup
public:

	static char* enum_wifi_mode_string(WiFiMode c);

	// --- CONSTRUCTOR ---
	Loom_WiFi_I(	char* module_name 		= "WiFi",

					WiFiMode 	mode		= WiFiMode::WPA_CLIENT,
					char* 		ssid		= "",
					char* 		pass		= "",
					uint 		UDP_port 	= 9411  	
				);

	// --- DESTRUCTOR ---
	virtual ~Loom_WiFi_I();


	void print_config();
	void print_state();
	void package(OSCBundle& bndl, char* suffix="") {}
	bool message_route(OSCMessage& msg, int address_offset) {}


	bool connect();
	bool is_connected();
	uint32_t get_time();




	bool connect_AP();
	bool connect_WPA(char* new_SSID="", char* new_pass="");

	void print_MAC(byte mac[]);

	// void print_remote_AP_device_status();

	// Should this be external?
	// bool log_to_pushingbox(OSCBundle& bndl);

private:

	/*static*/ uint LED_WiFi_connected[3] = {6, 50, 50};


};



#endif

