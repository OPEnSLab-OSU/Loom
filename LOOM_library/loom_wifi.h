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

#define AP_NAME   FAMILY DEVICE STR(INIT_INST)

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
	unsigned int devicePort;             // Local port to listen on
	unsigned int subnetPort; 
	byte        mac[6];                 // Device's MAC Address
	WiFiMode    wifi_mode;              // Devices current wifi mode
	bool        request_settings;        // True if device should request new channel settings on startup
};

struct state_wifi_t {
	// Global variables to handle changes to WiFi ssid and password 
	char new_ssid[32];
	char new_pass[32];
	bool ssid_set;
	bool pass_set;
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
void switch_to_AP(OSCMessage &msg);
void print_remote_mac_addr();
void replace_char(char *str, char orig, char rep);
void connect_to_new_network();
void set_ssid(OSCMessage &msg);
void set_pass(OSCMessage &msg);
void broadcastIP(OSCMessage &msg);
void set_port(OSCMessage &msg);
void wifi_check_status();
void request_settings_from_Max();
void set_request_settings(OSCMessage &msg);
void new_channel(OSCMessage &msg);
// void respond_to_poll_request(char packet_header_string[]);
void wifi_send_bundle(OSCBundle *bndl);
void wifi_send_bundle(OSCBundle *bndl, int port);
void wifi_send_bundle_subnet(OSCBundle *bndl);


void wifi_receive_bundle(OSCBundle *bndl, WiFiUDP *Udp, unsigned int port);
void clear_new_wifi_setting_buffers();
bool check_channel_poll(char * address_string, char * packet_header_string);
void check_connect_to_new_network();


void respond_to_poll_request(OSCMessage &msg);


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
					request_settings_from_Max();
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

	// printWiFiStatus();
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
	bndl->send(UdpDevice);    // Send the bytes to the SLIP stream
	UdpDevice.endPacket();        // Mark the end of the OSC Packet
}

// Version of wifi_send_bundle that will send on an arbitrary port
void wifi_send_bundle(OSCBundle *bndl, int port)
{
	UdpDevice.beginPacket(config_wifi->ip_broadcast, port);
	bndl->send(UdpDevice);    // Send the bytes to the SLIP stream
	UdpDevice.endPacket();        // Mark the end of the OSC Packet
}

// THE FOLLOWING TWO FUNCTIONS WILL  'HOPEFULLY' BECOME OBSOLETE 

void wifi_send_bundle_subnet(OSCBundle *bndl)
{
	UdpSubnet.beginPacket(config_wifi->ip_broadcast, config_wifi->subnetPort);
	bndl->send(UdpSubnet);    // Send the bytes to the SLIP stream
	UdpSubnet.endPacket();        // Mark the end of the OSC Packet
}

void wifi_send_bundle_global(OSCBundle *bndl)
{
	UdpGlobal.beginPacket(config_wifi->ip_broadcast, GLOBAL_PORT);				// Maybe this is called with new UDP object
	bndl->send(UdpGlobal);    // Send the bytes to the SLIP stream
	UdpGlobal.endPacket();        // Mark the end of the OSC Packet
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
	state_wifi.pass_set = false;
	state_wifi.ssid_set = false;
	
	// If there's data available, read a packet
	packetSize = Udp->parsePacket();

	if (packetSize > 0) {
		#if LOOM_DEBUG == 1
			if (packetSize > 0) {
				Serial.println("=========================================");
				Serial.print("Received packet of size: ");
				Serial.print(packetSize);
				Serial.print(" on port " );
				Serial.print(port);
				Serial.print(" (");
				Serial.print(type);
				Serial.println(")");
			}
		#endif
		
		bndl->empty();             // Empty previous bundle
		while (packetSize--){      // Read in new bundle
			bndl->fill(Udp->read());
		}
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
		Serial.print(config_wifi->mac[5], HEX);
		Serial.print(":");
		Serial.print(config_wifi->mac[4], HEX);
		Serial.print(":");
		Serial.print(config_wifi->mac[3], HEX);
		Serial.print(":");
		Serial.print(config_wifi->mac[2], HEX);
		Serial.print(":");
		Serial.print(config_wifi->mac[1], HEX);
		Serial.print(":");
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
		while (true);   // Don't continue
	}

	delay(10000);     // Wait 10 seconds for connection:
	
	server.begin();   // Start the web server on port 80

	#if LOOM_DEBUG == 1
		printWiFiStatus();   // You're connected now, so print out the status
		Serial.println("\nStarting UDP connection over server...");
	#endif
		
	LOOM_DEBUG_Println2("devicePort: ", config_wifi->devicePort);
	LOOM_DEBUG_Println2("subnetPort: ", config_wifi->subnetPort);

	// If you get a connection, report back via serial:
	UdpDevice.begin(config_wifi->devicePort);
	UdpSubnet.begin(config_wifi->subnetPort);
	UdpGlobal.begin(GLOBAL_PORT);
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
		LOOM_DEBUG_Println("Reverting to AP mode");

		// Start AP up again instead
		UdpDevice.stop();
		UdpSubnet.stop();
		UdpGlobal.stop();

		WiFi.disconnect();
		WiFi.end();
		start_AP();
		config_wifi->wifi_mode = AP_MODE;
		return false;
	}
	
	delay(8000); 
	


	#if LOOM_DEBUG == 1
		// You're connected now, so print out the status
		printWiFiStatus();
		Serial.println("Starting UDP connection over server...");
	#endif
	


	// If you get a connection, report back via serial:
	server.begin();

	UdpDevice.begin(config_wifi->devicePort);
	UdpSubnet.begin(config_wifi->subnetPort);
	UdpGlobal.begin(GLOBAL_PORT);

	return true;
}



// --- SWITCH TO ACCESS POINT ---
//
// Switch to AP mode upon OSC command to do so
// A wrapper for Start_AP  
// Saves AP_MODE as wifi_mode 
//
// @param msg  Not used, only there for msg_router to work properly
// Return:    none
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
	byte remoteMac[6];
	WiFi.APClientMacAddress(remoteMac);
			
	Serial.print("Device connected to AP, MAC address: ");
	Serial.print(  remoteMac[5], HEX); Serial.print(":");
	Serial.print(  remoteMac[4], HEX); Serial.print(":");
	Serial.print(  remoteMac[3], HEX); Serial.print(":");
	Serial.print(  remoteMac[2], HEX); Serial.print(":");
	Serial.print(  remoteMac[1], HEX); Serial.print(":");
	Serial.println(remoteMac[0], HEX); 
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



// --- CONNECT TO NEW NETWORK ---
//
// Attempt to connect WiFi network in client mode upon command to do so
// and both ssid and password provided
// Credentials stored in global wifi variable
// and copied into configuration struct and saved to non-volatile memory
// Reverts to AP mode upon failure
// 
void connect_to_new_network()
{
	// Replace '~'s with spaces - as spaces cannot be sent via Max and are replaced with '~'
	replace_char(state_wifi.new_ssid, '~', ' ');
	replace_char(state_wifi.new_pass, '~', ' ');

	LOOM_DEBUG_Print4("Received command to connect to ", state_wifi.new_ssid, " with password ", state_wifi.new_pass);

	// Disconnect from current WiFi network
	WiFi.disconnect();
	UdpDevice.stop();
	UdpSubnet.stop();
	UdpGlobal.stop();

	WiFi.end();
	
	// Try connecting on newly specified one
	// Will revert to AP Mode if this fails
	if(connect_to_WPA(state_wifi.new_ssid,state_wifi.new_pass)) {
		config_wifi->wifi_mode = WPA_CLIENT_MODE;
		config_wifi->ip = WiFi.localIP();
		strcpy(config_wifi->ssid, state_wifi.new_ssid);
		strcpy(config_wifi->pass, state_wifi.new_pass);
		write_non_volatile();
	} 
}


//////////////////////////////////////////////////////

// CAN (SHOULD) PROBABLY NOW JOIN THESE FUNCTIONS

// --- SET SSID ---
//
// Updates WiFi ssid global var with new ssid
// Sets global bool to indicate this 
//
// @param msg  OSC message with network ssid
//
void set_ssid(OSCMessage &msg) 
{
	msg.getString(0, state_wifi.new_ssid, 50);
	state_wifi.ssid_set = true;
}

// --- SET PASSWORD ---
//
// Updates WiFi password global var with new password 
// Sets global bool to indicate this 
//
// @param msg  OSC message with network password)
//
void set_pass(OSCMessage &msg) 
{
	msg.getString(0, state_wifi.new_pass, 50);
	state_wifi.pass_set = true;
}

//////////////////////////////////////////////////////





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

	// UdpSubnet.beginPacket(config_wifi->ip_broadcast, config_wifi->subnetPort);
	// bndl.send(UdpSubnet);     // Send the bytes to the SLIP stream
	// UdpSubnet.endPacket();    // Mark the end of the OSC Packet
	
	wifi_send_bundle_subnet(&bndl);
	bndl.empty();             // Empty the bundle to free room for a new one

	LOOM_DEBUG_Println2("Broadcasted IP: ", config_wifi->ip);
}



// --- SET PORT ---
//
// Update device's UDP communication port
// Port in configuration struct is update but not saved
//
// @param msg  OSC message of new port for device to communicate on
// 
void set_port(OSCMessage &msg) 
{
	LOOM_DEBUG_Print("Port changed from ");
	LOOM_DEBUG_Print(config_wifi->devicePort);

	// Get new port, stop listening on old port, start on new port
	config_wifi->devicePort = msg.getInt(0);
	UdpDevice.stop();
	UdpDevice.begin(config_wifi->devicePort);

	LOOM_DEBUG_Println2(" to ", config_wifi->devicePort);

	config_wifi->request_settings = 0;  // Setting to 0 means that device will not request new port settings on restart. 
										// Note that configuration needs to be saved for this to take effect

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
			if (status == WL_AP_CONNECTED) { 	// A device has connected to the AP
				print_remote_mac_addr();                            
			} else {							// A device has disconnected from the AP, and we are back in listening mode 
				Serial.println("Device disconnected from AP");
			}
		#endif
	} // of if ( status != WiFi.status() )
}



// --- REQUEST SETTINGS FROM MAX ---
//
// Used to request channel settings from Max Channel Manager
// Setting response will entail a SetChannel command 
//
void request_settings_from_Max()
{
	OSCBundle bndl;
	char address_string[80];
	sprintf(address_string, "%s%s", packet_header_string, "/RequestSettings");

	bndl.add(address_string);

	wifi_send_bundle_subnet(&bndl);
	bndl.empty();             // Empty the bundle to free room for a new one

	LOOM_DEBUG_Println("Requested New Channel Settings");
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
void new_channel(OSCMessage &msg)
{
	LOOM_DEBUG_Println("Received Command to get new channel settings");
	request_settings_from_Max();
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
// void respond_to_poll_request(char packet_header_string[])
void respond_to_poll_request(OSCMessage &msg)
{
	OSCBundle bndl;
	bndl.empty();
	char address_string[80];
	sprintf(address_string, "%s%s", packet_header_string, "/PollResponse");

	bndl.add(address_string);

	UdpSubnet.beginPacket(config_wifi->ip_broadcast, config_wifi->subnetPort);
	bndl.send(UdpSubnet);     // Send the bytes to the SLIP stream
	UdpSubnet.endPacket();    // Mark the end of the OSC Packet
	bndl.empty();             // Empty the bundle to free room for a new one

	LOOM_DEBUG_Println("Responded to poll request");
}



void clear_new_wifi_setting_buffers() 
{
	for (int i = 0; i < 32; i++) {  
		state_wifi.new_ssid[i] = '\0';
		state_wifi.new_pass[i] = '\0';
	}
}


void check_connect_to_new_network()
{
	if (state_wifi.ssid_set == true && state_wifi.pass_set == true) {
		connect_to_new_network();   
	}
}
	

