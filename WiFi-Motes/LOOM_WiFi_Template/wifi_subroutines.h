

// Function prototypes
bool connect_to_WPA(char ssid[], char pass[]);
void start_AP();


// Function called by setup()
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
  switch(configuration.wifi_mode){
    case AP_MODE:
      start_AP();
      break;
    case WPA_CLIENT_MODE:
      if (connect_to_WPA(configuration.ssid,configuration.pass)){
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
  configuration.ip = WiFi.localIP();
}



/* Print WiFi Status
   If debug enabled, display WiFi settings / state to serial
*/
void printWiFiStatus() 
{
  #if DEBUG == 1
    // Print the SSID of the network you're attached to:
    Serial.print("SSID: "); 
    Serial.println(WiFi.SSID());
  
    // Print your WiFi shield's IP address:
    configuration.ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(configuration.ip);
  
    // Print your MAC address:
    WiFi.macAddress(configuration.mac);
    Serial.print("MAC address: ");
    Serial.print(configuration.mac[5], HEX);
    Serial.print(":");
    Serial.print(configuration.mac[4], HEX);
    Serial.print(":");
    Serial.print(configuration.mac[3], HEX);
    Serial.print(":");
    Serial.print(configuration.mac[2], HEX);
    Serial.print(":");
    Serial.print(configuration.mac[1], HEX);
    Serial.print(":");
    Serial.println(configuration.mac[0], HEX);
  
    // Print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
    
    // Print where to go in a browser:
    Serial.print("To see this page in action, open a browser to http://");
    Serial.println(configuration.ip);
  #endif // of DEBUG
}



/* Start AP
   Device will setup wifi access point for one computer to connect to
*/
void start_AP() 
{
  #if DEBUG == 1
    // print the network name (SSID);
    Serial.print("Creating access point named: ");
    Serial.println(configuration.my_ssid);
  #endif

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(configuration.my_ssid);
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
  Udp.begin(configuration.localPort);
}




/*
   Connect to WPA
   Returns true is connection successful, false on failure (will revert to AP mode)
*/
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
    configuration.wifi_mode = AP_MODE;
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
  Udp.begin(configuration.localPort);
  return true;
}


// Switch to AP mode upon OSC command to do so
void switch_to_AP(OSCMessage &msg) 
//void switch_to_AP() 
{
  if (configuration.wifi_mode != AP_MODE) {
    #if DEBUG == 1
        Serial.println("Received command to switch to AP mode");
    #endif
    
    Udp.stop();
    WiFi.disconnect();
    WiFi.end();
    start_AP();
    configuration.wifi_mode = AP_MODE;
    flash_config.write(configuration);
  }
  
  #if DEBUG == 1
  else {
    Serial.println("Already in AP mode, no need to switch");
  }
  #endif
}

// Prints the MAC address of device connected to devices access point
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





void connect_to_new_network()
{
  // Replace '~'s with spaces - as spaces cannot be sent via Max and are replaced with '~'
  replace_char(new_ssid, '\0', '~');
  replace_char(new_pass, '\0', '~');

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
    configuration.wifi_mode = WPA_CLIENT_MODE;
    configuration.ip = WiFi.localIP();
    strcpy(configuration.ssid,new_ssid);
    strcpy(configuration.pass,new_pass);
    flash_config.write(configuration);
  } 
}




