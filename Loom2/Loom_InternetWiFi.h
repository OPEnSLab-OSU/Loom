
#ifndef LOOM_INTERNET_WIFI_PLAT_h
#define LOOM_INTERNET_WIFI_PLAT_h

#include "Loom_InternetPlat.h"

#include <WiFi101.h>
#include <WiFiUdp.h>


enum WiFiMode {
	AP_MODE,
	WPA_CLIENT_MODE,
	WEP_CLIENT_MODE
};



class Loom_WiFi_I : public LoomInternetPlat
{

protected:
	
	IPAddress   ip;                     // Device's IP Address
	char*       my_ssid;                // Default AP name
	char        ssid[32];               // Host network name
	char        pass[32];               // Host network password
	// int         keyIndex;               // Key Index Number (needed only for WEP)
	char*       ip_broadcast;           // IP to Broadcast data
	unsigned int devicePort;            // Local port to listen device specific messages on
	unsigned int subnetPort; 			// Local port to listen for family subnet messages on	
	byte        mac[6];                 // Device's MAC Address
	WiFiMode    wifi_mode;              // Devices current wifi mode
	// bool        request_settings;       // True if device should request new channel settings on startup
	
public:

	static char* enum_wifi_mode_string(WiFiMode c);


	// --- CONSTRUCTOR ---
	Loom_WiFi_I(	char* module_name 	= "WiFi",

					char* ssid 			= "",
					char* pass 			= ""



					);

	// --- DESTRUCTOR ---
	virtual ~Loom_WiFi_I();


	// --- PUBLIC METHODS ---

	void print_config();
	void print_state();

	bool connect();
	bool is_connected();
	uint32_t get_time();



	// Should this be external?
	// bool log_to_pushingbox(OSCBundle& bndl);

private:

	bool connect_AP();
	bool connect_WPA();


};



#endif

