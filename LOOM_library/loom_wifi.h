// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <WiFi101.h>
#include <WiFiUdp.h>


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================ 
enum WiFiMode {
	AP_MODE,
	WPA_CLIENT_MODE,
	WEP_CLIENT_MODE
};

#define AP_NAME   FAMILY STR(FAMILY_NUM) STR(_) DEVICE STR(INIT_INST)


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct config_wifi_t{
	IPAddress   ip;                     // Device's IP Address
	char*       my_ssid;                // Default AP name
	char        ssid[32];               // Host network name
	char        pass[32];               // Host network password
	int         keyIndex;               // Key Index Number (needed only for WEP)
	char*       ip_broadcast;           // IP to Broadcast data
	unsigned int devicePort;            // Local port to listen device specific messages on
	unsigned int subnetPort; 			// Local port to listen for family subnet messages on	
	byte        mac[6];                 // Device's MAC Address
	WiFiMode    wifi_mode;              // Devices current wifi mode
	bool        request_settings;       // True if device should request new channel settings on startup
};

struct state_wifi_t {
	// Global variables to handle changes to WiFi ssid and password 
	char new_ssid[32];
	char new_pass[32];
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

struct config_wifi_t * config_wifi;
void link_config_wifi(struct config_wifi_t *flash_setup_wifi){
	config_wifi = flash_setup_wifi;
}

struct state_wifi_t state_wifi;

char * packet_header_string;

// WiFi global vars/structs
WiFiUDP 	UdpDevice;
WiFiUDP 	UdpSubnet;			
WiFiUDP 	UdpGlobal;
WiFiServer   server(80);
int status = WL_IDLE_STATUS;

#if is_adafruitio == 1 || is_pushingbox == 1
	WiFiClient wifi_client;
#endif


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_wifi(char packet_header_string[]);
void printWiFiStatus();
void start_AP();
bool connect_to_WPA(char ssid[], char pass[]);
void connect_to_new_wifi_network(OSCMessage &msg);
void print_remote_mac_addr();
void replace_char(char *str, char orig, char rep);
void wifi_check_status();
void wifi_send_bundle(OSCBundle *bndl);
void wifi_send_bundle(OSCBundle *bndl, int port);
void wifi_receive_bundle(OSCBundle *bndl, WiFiUDP *Udp, unsigned int port);

// Routed from msg_router
void switch_to_AP(OSCMessage &msg);
void broadcastIP(OSCMessage &msg);
void get_new_channel(OSCMessage &msg);
void set_request_settings(OSCMessage &msg);
void respond_to_poll_request(OSCMessage &msg);

uint32_t get_time_wifi();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================

// --- SETUP WIFI ---
//
// Called by main setup
// Sets WiFi pins, checks for shield with WiFi, 
// starts AP mode or tries to connect to existing network
// Requests channel settings from if Request Settings is set
// Otherwise it broadcasts its channel information
//
// @param packet_header_string  The device-identifying string to prepend to OSC messages
//
void setup_wifi(char packet_header_string[])
{
	LOOM_DEBUG_Println("Setting up WiFi");

	// Configure pins for Adafruit ATWINC1500 Feather
	WiFi.setPins(8,7,4,2);      
		
	// Check for the presence of the shield, else don't continue:
	if (WiFi.status() == WL_NO_SHIELD) {
		LOOM_DEBUG_Println("WiFi shield not present, entering infinite loop");
		while (true); 
	}

	// By default the local IP address of will be 192.168.1.1
	// You can override it with the following:
	// WiFi.config(IPAddress(192, 168, 1, 10));
	switch(config_wifi->wifi_mode) {
		case AP_MODE:
			start_AP();
			break;
		case WPA_CLIENT_MODE:
			if (connect_to_WPA(config_wifi->ssid,config_wifi->pass)){
				LOOM_DEBUG_Println("Success!");

				// If set to request channel settings
				if (config_wifi->request_settings == 1) {
					OSCMessage tmp = new OSCMessage("tmp");
					get_new_channel(tmp);
				} else {
					OSCMessage tmp = new OSCMessage("tmp");
					respond_to_poll_request(tmp);
				}
			}
			else {
				LOOM_DEBUG_Println("Failure :(");
				while(true);
			}

			// This is being used for pushingbox testing
	// This is commented out because it was causing deleted ip addresses
	// Maybe I can just move it up
	// Or maybe this 'WiFi' class has an IP member as well that is preserved
			// WiFi.begin(config_wifi->ssid, config_wifi->pass);

			break;
	}

// This was being used to check if the IP address still existed after WiFi.begin()
	// IPAddress ip = WiFi.localIP();                
	// LOOM_DEBUG_Print("WIFI IP: ");
	// Serial.println(ip);

	config_wifi->ip = WiFi.localIP();
	LOOM_DEBUG_Println("Finished setting up WiFi.");

	OSCMessage tmp = new OSCMessage("tmp");	
	broadcastIP(tmp);

	printWiFiStatus();
}




// ================================================================ 
// ===              WIFI SEND AND RECEIVE BUNDLE                === 
// ================================================================
// --- WIFI SEND BUNDLE ---
//
// This is a simple helper function to encapsulate the 
// common sequence of commands to send an OSC bundle
// Note that this send on the device unique UDP port, devicePort,
// not the common port, 9440
//
// @param bndl  The OSC bundle to send on the device unique port
//
void wifi_send_bundle(OSCBundle *bndl)
{
	UdpDevice.beginPacket(config_wifi->ip_broadcast, config_wifi->devicePort);
	bndl->send(UdpDevice);    	// Send the bytes to the SLIP stream
	UdpDevice.endPacket();		// Mark the end of the OSC Packet
}

// Version of wifi_send_bundle that will send on a specified port
void wifi_send_bundle(OSCBundle *bndl, int port)
{
	UdpDevice.beginPacket(config_wifi->ip_broadcast, port);
	bndl->send(UdpDevice);    	// Send the bytes to the SLIP stream
	UdpDevice.endPacket();		// Mark the end of the OSC Packet
}


// --- WIFI RECEIVE BUNDLE ---
//
// Function that fills an OSC Bundle with packets from UDP
// Routes messages to correct function via msg_router if message header string matches expected
//
// @param bndl                  OSC bundle to be filled
// @param packet_header_string  Header string to route messages on, as there are unique ports and a common port
// @param Udp                   Which WiFIUdp structure to read packets from
// @param port                  Which port the packet was received on, used primarily for debug prints
//
void wifi_receive_bundle(OSCBundle *bndl, WiFiUDP *Udp, unsigned int port, char * type)
{  
	int packetSize; 
	
	// If there's data available, read a packet
	packetSize = Udp->parsePacket();

	if (packetSize > 0) {
		LOOM_DEBUG_Println("=========================================");
		LOOM_DEBUG_Print2("Received packet of size: ", packetSize);
		LOOM_DEBUG_Print2(" on port ", port);
		LOOM_DEBUG_Println3(" (", type, ")");
		
		// LOOM_DEBUG_Println("WIFI BUNDLE A");
		// print_bundle(bndl);

		bndl->empty();             // Empty previous bundle
		while (packetSize--){      // Read in new bundle
			bndl->fill(Udp->read());
		}

		// LOOM_DEBUG_Println("WIFI BUNDLE B");
		// print_bundle(bndl);
	} // of (packetSize > 0)

}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================

// --- PRINT WIFI STATUS ---
//
// If debug enabled, display WiFi settings / state to serial
// Does nothing else
//
void printWiFiStatus() 
{
	#if LOOM_DEBUG == 1
		// Print the SSID of the network you're attached to:
		Serial.print("SSID: "); 
		Serial.println(WiFi.SSID());
	
		// Print your WiFi shield's IP address:
		config_wifi->ip = WiFi.localIP();
		Serial.print("IP Address: ");
		Serial.println(config_wifi->ip);
	
		// Print your MAC address:
		WiFi.macAddress(config_wifi->mac);
		Serial.print("MAC address: ");
		Serial.print(config_wifi->mac[5], HEX); Serial.print(":");
		Serial.print(config_wifi->mac[4], HEX); Serial.print(":");
		Serial.print(config_wifi->mac[3], HEX); Serial.print(":");
		Serial.print(config_wifi->mac[2], HEX); Serial.print(":");
		Serial.print(config_wifi->mac[1], HEX); Serial.print(":");
		Serial.println(config_wifi->mac[0], HEX);
	
		// Print the received signal strength:
		long rssi = WiFi.RSSI();
		Serial.print("signal strength (RSSI):");
		Serial.print(rssi);
		Serial.println(" dBm");
		
		// Print where to go in a browser:
		Serial.print("To see this page in action, open a browser to http://");
		Serial.println(config_wifi->ip);
	#endif // of LOOM_DEBUG
}



// --- START ACCESS POINT ---
//
// Device will attempt to setup wifi access point that one computer to connect to
// Called upon OSC command, button held, or connect to network failure
//
void start_AP() 
{
	// Print the network name (SSID);
	LOOM_DEBUG_Println2("Creating access point named: ", config_wifi->my_ssid);

	// Create open network. Change this line if you want to create an WEP network:
	status = WiFi.beginAP(config_wifi->my_ssid);
	if (status != WL_AP_LISTENING) {
		LOOM_DEBUG_Println("Creating access point failed");
		while (true);   	// Don't continue
	}

	delay(10000);     		// Wait 10 seconds for connection:
	
	server.begin();   		// Start the web server on port 80

	printWiFiStatus();   	// You're connected now, so print out the status
	LOOM_DEBUG_Println("\nStarting UDP connection over server...");
		
	// If you get a connection, report back via serial:
	UdpDevice.begin(config_wifi->devicePort);

	LOOM_DEBUG_Println("Done");
	flash_led(6, 50, 50);
}



// --- CONNECT TO WPA ---
//
// Device tries to connect to network with provide credentials 
// Will revert to AP mode if this fails
//
// @param ssid (Wifi network name), pass (Wifi network password)
//
// @return  Whether connection was successful
//
bool connect_to_WPA(char ssid[], char pass[]) 
{
	status = WiFi.begin(ssid, pass);
	int attempt_count = 0;

	// Try to connect, attempting the connection up to 10 times (this number is arbitrary)
	while (status != WL_CONNECTED && attempt_count < 10) {
		LOOM_DEBUG_Println("Connecting to WPA host failed, trying again");
		
		status = WiFi.begin(ssid, pass);
		attempt_count++;
	}

	// If not successfully connected
	if (status != WL_CONNECTED) {
		LOOM_DEBUG_Println("Connecting to WPA host failed completely");
		LOOM_DEBUG_Println("Reverting to previous wifi network, else AP mode");

		// Start AP up again instead
		UdpDevice.stop();
		UdpSubnet.stop();
		UdpGlobal.stop();
		WiFi.disconnect();
		WiFi.end();

		// Restart in previous configuration
		if (config_wifi->wifi_mode == AP_MODE) {
			LOOM_DEBUG_Println("Try to revert to AP");
			start_AP();
			return false;

		} else {
			LOOM_DEBUG_Println("Try to revert to WPA");
			status = WiFi.begin(config_wifi->ssid, config_wifi->pass);
			if (status != WL_CONNECTED) {
				LOOM_DEBUG_Println("Revert to previous WPA network failed");
				start_AP();
				return false;
			}
		}

	}
	
	delay(8000); 

	// You're connected now, so print out the status
	printWiFiStatus();
	LOOM_DEBUG_Println("Starting UDP connection over server...");

	// If you get a connection, report back via serial:
	server.begin();

	UdpDevice.begin(config_wifi->devicePort);
	UdpSubnet.begin(config_wifi->subnetPort);
	UdpGlobal.begin(GLOBAL_PORT);

	OSCMessage tmp = new OSCMessage("tmp");	
	broadcastIP(tmp);

	LOOM_DEBUG_Println("Done");
	flash_led(6, 50, 50);

	return true;
}



// --- CONNECT TO NEW WIFI NETWORK ---
//
// Updates WiFi password global var with new password 
// Sets global bool to indicate this 
//
// @param msg  OSC message with network password)
//
void connect_to_new_wifi_network(OSCMessage &msg) 
{
	memset(state_wifi.new_ssid, '\0', sizeof(state_wifi.new_ssid));
	memset(state_wifi.new_pass, '\0', sizeof(state_wifi.new_pass));

	msg.getString(0, state_wifi.new_ssid, sizeof(state_wifi.new_ssid));
	msg.getString(1, state_wifi.new_pass, sizeof(state_wifi.new_ssid));

	// Replace '~'s with spaces - as spaces cannot be sent via Max and are replaced with '~'
	replace_char(state_wifi.new_ssid, '~', ' ');
	replace_char(state_wifi.new_pass, '~', ' ');

	LOOM_DEBUG_Print2("Received command to connect to '", state_wifi.new_ssid);
	LOOM_DEBUG_Println3("' with password '", state_wifi.new_pass, "'");

	// Disconnect from current WiFi network
	UdpDevice.stop();
	UdpSubnet.stop();
	UdpGlobal.stop();
	WiFi.disconnect();
	WiFi.end();
	
	// Try connecting on newly specified one
	// Will revert to AP Mode if this fails
	// Only save the new settings if they worked
	if (connect_to_WPA(state_wifi.new_ssid,state_wifi.new_pass)) {
		config_wifi->wifi_mode = WPA_CLIENT_MODE;
		config_wifi->ip = WiFi.localIP();
		strcpy(config_wifi->ssid, state_wifi.new_ssid);
		strcpy(config_wifi->pass, state_wifi.new_pass);
		write_non_volatile();
	} 
}



// --- SWITCH TO ACCESS POINT ---
//
// Switch to AP mode upon OSC command to do so
// A wrapper for Start_AP  
// Saves AP_MODE as wifi_mode 
//
// @param msg  Not used, only there for msg_router to work properly
//
void switch_to_AP(OSCMessage &msg) 
{
	if (config_wifi->wifi_mode != AP_MODE) {
		LOOM_DEBUG_Println("Received command to switch to AP mode");
		
		UdpDevice.stop();
		UdpSubnet.stop();
		UdpGlobal.stop();
		WiFi.disconnect();
		WiFi.end();

		start_AP();

		config_wifi->wifi_mode = AP_MODE;
		write_non_volatile();
	}
	
	#if LOOM_DEBUG == 1
	else {
		Serial.println("Already in AP mode, no need to switch");
	}
	#endif
}



// --- PRINT REMOVE MAC ADDRESS ---
//
// Prints the MAC address of device connected to devices access point
// 
void print_remote_mac_addr()
{
	#if LOOM_DEBUG == 1
		byte remoteMac[6];
		WiFi.APClientMacAddress(remoteMac);
				
		Serial.print("Device connected to AP, MAC address: ");
		Serial.print(  remoteMac[5], HEX); Serial.print(":");
		Serial.print(  remoteMac[4], HEX); Serial.print(":");
		Serial.print(  remoteMac[3], HEX); Serial.print(":");
		Serial.print(  remoteMac[2], HEX); Serial.print(":");
		Serial.print(  remoteMac[1], HEX); Serial.print(":");
		Serial.println(remoteMac[0], HEX); 
	#endif
}



// --- REPLACE CHARACTER ---
//
// Given a string, replace all instances of 'orig' char with 'rep' char
// Used primarily for replacing '~'s sent by Max
// as it cannot send strings with spaces
//
// @param str   Pointer to string to alter
// @param orig  Character to replace
// @param rep   Replacement character
// 
void replace_char(char *str, char orig, char rep) 
{
	char *ix = str;
	while((ix = strchr(ix, orig)) != NULL) {
		*ix++ = rep;
	}
}



// --- BROADCAST IP ---
//
// Broadcasts IP address so that requesting computer can update IP
// to send to if it only had device instance number 
//
// @param msg  OSC message with no data, only message header was needed by msg_router()
// 
void broadcastIP(OSCMessage &msg) 
{
	OSCBundle bndl;
	config_wifi->ip = WiFi.localIP();
	char address_string[80];
	sprintf(address_string, "/%s%d%s", FAMILY, FAMILY_NUM, "/NewIP");

	bndl.add(address_string).add( packet_header_string )
						    .add( (int32_t)config_wifi->ip[0] )
						    .add( (int32_t)config_wifi->ip[1] )
						    .add( (int32_t)config_wifi->ip[2] )
						    .add( (int32_t)config_wifi->ip[3] ) ;

wifi_send_bundle(&bndl, config_wifi->subnetPort);
    // wifi_send_bundle(&bndl);
	bndl.empty();		// Empty the bundle to free room for a new one

	LOOM_DEBUG_Println2("Broadcasted IP: ", config_wifi->ip);
}



// --- WIFI CHECK STATUS --- 
//
// Used to check if remote computer is still 
// connected to this device's access point
//
void wifi_check_status()
{
	// Compare the previous status to the current status
	if (status != WiFi.status()) {
		status = WiFi.status();					// It has changed, update the variable
		
		#if LOOM_DEBUG == 1
			if (status == WL_AP_CONNECTED) { 	// A computer has connected to the AP
				print_remote_mac_addr();                            
			} else {							// A computer has disconnected from the AP, and we are back in listening mode 
				LOOM_DEBUG_Println("Device disconnected from AP");
			}
		#endif
	} // of if ( status != WiFi.status() )
}



// --- SET REQUEST SETTINGS --- 
//
// Sets the 'Request_Settings' option in the configuration
// On next startup, device will request channel settings from Max
//
// @param msg  OSC messages that had header ending in '/SetRequestSettings'
//				only used by msg_router(), not used here
//
void set_request_settings(OSCMessage &msg)
{
	config_wifi->request_settings = 1; // Setting to 1 means that device will request new port settings on restart. 
	write_non_volatile();
	LOOM_DEBUG_Println("Setting Request Settings True");
}



// --- NEW CHANNEL ---
//
// Called if device received command to request
// channel settings from Max Channel Manager
// Setting response will entail SetID and SetPort commands 
//
// @param msg  OSC messages that had header ending in '/getNewChannel'
//				only used by msg_router(), not used here
//
void get_new_channel(OSCMessage &msg)
{
	LOOM_DEBUG_Println("Received Command to get new channel settings");

	OSCBundle bndl;
	char address_string[80];
	sprintf(address_string, "%s%s", packet_header_string, "/RequestSettings");
	bndl.add(address_string);

wifi_send_bundle(&bndl, config_wifi->subnetPort);
	// wifi_send_bundle(&bndl);

	bndl.empty();		// Empty the bundle to free room for a new one

	LOOM_DEBUG_Println("Requested New Channel Settings");
}



// --- RESPOND TO POLL REQUEST ---
// 
// Device received poll request from Max Channel Manager to respond presence
// The function is not routed to by msg_router() but directly from 
// wifi_receive_bundle() as a poll request comes on the common UDP port, 9440, 
// not the device unique port
// 
// @param packet_header_string  The device-identifying string to prepend to OSC messages
// 
void respond_to_poll_request(OSCMessage &msg)
{
	OSCBundle bndl;
	bndl.empty();
	char address_string[80];
	sprintf(address_string, "%s%s", packet_header_string, "/PollResponse");
	bndl.add(address_string);

	wifi_send_bundle(&bndl, config_wifi->subnetPort);
	// wifi_send_bundle(&bndl);
	bndl.empty();		// Empty the bundle to free room for a new one

	LOOM_DEBUG_Println("Responded to poll request");
}



// --- GET TIME WIFI ---
//
// Gets the UTC time over internet
//
// @return Unix time (seconds since 1970)
//
uint32_t get_time_wifi()
{
	return WiFi.getTime();
}

