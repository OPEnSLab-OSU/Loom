#if is_wifi == 1
  enum WiFiMode {
    AP_MODE,
    WPA_CLIENT_MODE,
    WEP_CLIENT_MODE
  };
  
  #include <WiFi101.h>
  #include <WiFiUdp.h>

  #define AP_NAME   STR(FAMILY) STR(DEVICE) STR(INIT_INST)
  struct config_wifi_t{
    IPAddress   ip;                     // Device's IP Address
    char*       my_ssid;                // Default AP name
    char        ssid[32];               // Host network name
    char        pass[32];               // Host network password
    int         keyIndex;               // Key Index Number (needed only for WEP)
    char*       ip_broadcast;           // IP to Broadcast data
    unsigned int localPort;             // Local port to listen on
    byte        mac[6];                 // Device's MAC Address
    WiFiMode    wifi_mode;              // Devices current wifi mode
  };
  struct config_wifi_t * config_wifi;
  void link_config_wifi(struct config_wifi_t *flash_config_wifi){
    config_wifi = flash_config_wifi;
  }
  char * packet_header_string;
  // WiFi global vars/structs
  WiFiUDP      Udp;
  WiFiServer   server(80);
  int status = WL_IDLE_STATUS;

  // Global variables to handle changes to WiFi ssid and password 
  char new_ssid[32];
  char new_pass[32];
  bool ssid_set;
  bool pass_set;
#endif // of is_wifi


// Function prototypes
bool connect_to_WPA(char ssid[], char pass[]);
void start_AP();



// --- WIFI SETUP ---
// Called by main setup
// Sets WiFi pins, checks for shield with WiFi, 
// starts AP mode or tries to connect to existing network
// Arguments: none
// Return:    none
void wifi_setup()
{
  // Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);      
    
  // Check for the presence of the shield, else don't continue:
  if (WiFi.status() == WL_NO_SHIELD) {
    #if DEBUG == 1
      Serial.println("WiFi shield not present, entering infinite loop");
    #endif
    while (true); 
  }

  // By default the local IP address of will be 192.168.1.1
  // You can override it with the following:
  // WiFi.config(IPAddress(192, 168, 1, 10));
  switch(config_wifi->wifi_mode){
    case AP_MODE:
      start_AP();
      break;
    case WPA_CLIENT_MODE:
      if (connect_to_WPA(config_wifi->ssid,config_wifi->pass)){
        #if DEBUG == 1
        Serial.println("Success!");
        #endif
      }
      else {
        #if DEBUG == 1
        Serial.println("Failure :(");
        #endif
        while(true);
      }
      break;
  }
  config_wifi->ip = WiFi.localIP();
}



// --- PRINT WIFI STATUS ---
// If debug enabled, display WiFi settings / state to serial
// Arguments: none
// Return:    none
void printWiFiStatus() 
{
  #if DEBUG == 1
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
  #endif // of DEBUG
}



// --- START ACCESS POINT ---
// Device will attempt to setup wifi access point that one computer to connect to
// Called upon OSC command, button held, or connect to network failure
// Arguments: none
// Return:    none
void start_AP() 
{
  #if DEBUG == 1
    // print the network name (SSID);
    Serial.print("Creating access point named: ");
    Serial.println(config_wifi->my_ssid);
  #endif

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(config_wifi->my_ssid);
  if (status != WL_AP_LISTENING) {
    #if DEBUG == 1
        Serial.println("Creating access point failed");
    #endif
    while (true);   // Don't continue
  }

  delay(10000);     // Wait 10 seconds for connection:
  
  server.begin();   // Start the web server on port 80

  #if DEBUG == 1
    printWiFiStatus();   // You're connected now, so print out the status
    Serial.println("\nStarting UDP connection over server...");
  #endif
  
  // If you get a connection, report back via serial:
  Udp.begin(config_wifi->localPort);
}



// --- CONNECT TO WPA ---
// Device tries to connect to network with provide credentials 
// Will revert to AP mode if this fails
// Arguments: ssid (Wifi network name), pass (Wifi network password)
// Return: bool (true is connection successful, false on failure )
bool connect_to_WPA(char ssid[], char pass[]) 
{
  status = WiFi.begin(ssid, pass);
  int attempt_count = 0;

  // Try to connect, attempting the connection up to 10 times (this number is arbitrary)
  while (status != WL_CONNECTED && attempt_count < 10) {
    #if DEBUG == 1
      Serial.println("Connecting to WPA host failed, trying again");
    #endif
    
    status = WiFi.begin(ssid, pass);
    attempt_count++;
  }

  // If not successfully connected
  if (status != WL_CONNECTED) {
    #if DEBUG == 1
      Serial.println("Connecting to WPA host failed completely");
      Serial.println("Reverting to AP mode");
    #endif

    // Start AP up again instead
    Udp.stop();
    WiFi.disconnect();
    WiFi.end();
    start_AP();
    config_wifi->wifi_mode = AP_MODE;
    return false;
  }
  
  delay(8000); 
  
  #if DEBUG == 1
    // You're connected now, so print out the status
    printWiFiStatus();
    Serial.println("Starting UDP connection over server...");
  #endif
  
  // If you get a connection, report back via serial:
  server.begin();
  Udp.begin(config_wifi->localPort);
  return true;
}



// --- SWITCH TO ACCESS POINT ---
// Switch to AP mode upon OSC command to do so
// Arguments: msg (not used, only there for msg_router to work properly)
// Return:    none
void switch_to_AP(OSCMessage &msg) 
{
  if (config_wifi->wifi_mode != AP_MODE) {
    #if DEBUG == 1
        Serial.println("Received command to switch to AP mode");
    #endif
    
    Udp.stop();
    WiFi.disconnect();
    WiFi.end();
    start_AP();
    config_wifi->wifi_mode = AP_MODE;
    //flash_config.write(configuration);
  }
  
  #if DEBUG == 1
  else {
    Serial.println("Already in AP mode, no need to switch");
  }
  #endif
}


// --- PRINT REMOVE MAC ADDRESS ---
// Prints the MAC address of device connected to devices access point
// Arguments: none
// Return:    none
void print_remote_mac_addr()
{
  byte remoteMac[6];
  WiFi.APClientMacAddress(remoteMac);
      
  Serial.print("Device connected to AP, MAC address: ");
  Serial.print(remoteMac[5], HEX);
  Serial.print(":");
  Serial.print(remoteMac[4], HEX);
  Serial.print(":");
  Serial.print(remoteMac[3], HEX);
  Serial.print(":");
  Serial.print(remoteMac[2], HEX);
  Serial.print(":");
  Serial.print(remoteMac[1], HEX);
  Serial.print(":");
  Serial.println(remoteMac[0], HEX); 
}



// --- REPLACE CHARACTER ---
// Given a string, replace all instances of 'orig' char with 'rep' char
// Used primarily for replacing '~'s sent by Max, as it cannot send strings with spaces
// Arguments: str (pointer to string to alter), orig (character to replace), rep (replacement character)
// Return:    none
void replace_char(char *str, char orig, char rep) {
    char *ix = str;
    while((ix = strchr(ix, orig)) != NULL) {
        *ix++ = rep;
    }
}



// --- CONNECT TO NEW NETWORK ---
// Attempt to connect WiFi network in client mode upon command to do so
// and both ssid and password provided
// Credentials stored in global variable
// Reverts to AP mode upon failure
// Arguments: none 
// Return:    none
void connect_to_new_network()
{
  // Replace '~'s with spaces - as spaces cannot be sent via Max and are replaced with '~'
  replace_char(new_ssid, '~', ' ');
  replace_char(new_pass, '~', ' ');

  #if DEBUG == 1
    Serial.print("received command to connect to ");
    Serial.print(new_ssid);
    Serial.print(" with password ");
    Serial.println(new_pass);
  #endif

  // Disconnect from current WiFi network
  WiFi.disconnect();
  Udp.stop();
  WiFi.end();
  
  // Try connecting on newly specified one
  // Will revert to AP Mode if this fails
  if(connect_to_WPA(new_ssid,new_pass)) {
    config_wifi->wifi_mode = WPA_CLIENT_MODE;
    config_wifi->ip = WiFi.localIP();
    strcpy(config_wifi->ssid,new_ssid);
    strcpy(config_wifi->pass,new_pass);
    //flash_config.write(configuration);
  } 
}



// --- SET SSID ---
// Updates WiFi ssid global var with new ssid
// Sets global bool to indicate this 
// Arguments: msg (OSC message with network ssid)
// Return:    none
void set_ssid(OSCMessage &msg) 
{
  msg.getString(0, new_ssid, 50);
  ssid_set = true;
}



// SET PASSWORD
// Updates WiFi password global var with new password 
// Sets global bool to indicate this 
// Arguments: msg (OSC message with network password)
// Return:    none
void set_pass(OSCMessage &msg) 
{
  msg.getString(0, new_pass, 50);
  pass_set = true;
}



// --- BROADCAST IP ---
// Broadcast IP address so that requesting computer can update IP
// to send to if it only had device instance number 
// Arguments: msg (OSC message with no data, only message header was needed by msg_router())
// Return:    none
void broadcastIP(OSCMessage &msg) {
  OSCBundle bndl;
  config_wifi->ip = WiFi.localIP();
  char addressString[255];
  sprintf(addressString, "%s%s", packet_header_string, "/NewIP");
  bndl.add(addressString).add((int32_t)config_wifi->ip[0])
                         .add((int32_t)config_wifi->ip[1])
                         .add((int32_t)config_wifi->ip[2])
                         .add((int32_t)config_wifi->ip[3]);

  Udp.beginPacket(config_wifi->ip_broadcast, config_wifi->localPort);
  bndl.send(Udp);     // Send the bytes to the SLIP stream
  Udp.endPacket();    // Mark the end of the OSC Packet
  bndl.empty();       // Empty the bundle to free room for a new one

  #if DEBUG == 1
    Serial.print("Broadcasted IP: ");
    Serial.println(config_wifi->ip);
  #endif
}



// --- SET PORT ---
// Update device's communication port
// Arguments: msg (OSC message of new port for device to communicate on)
// Return:    none
void set_port(OSCMessage &msg) 
{
  #if DEBUG == 1
    Serial.print("Port changed from ");
    Serial.print(config_wifi->localPort);
  #endif

  // Get new port, stop listening on old port, start on new port
  config_wifi->localPort = msg.getInt(0);
  Udp.stop();
  Udp.begin(config_wifi->localPort);

  #if DEBUG == 1
    Serial.print(" to ");
    Serial.println(config_wifi->localPort);
  #endif

  // Update saved port in configuration
  //flash_config.write(configuration);
}



