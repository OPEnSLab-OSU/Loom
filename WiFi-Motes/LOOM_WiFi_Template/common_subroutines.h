
extern void switch_to_AP(OSCMessage &msg);
//extern transmit_butt;

// Update device's instance number
void set_instance_num(OSCMessage &msg) 
{
  configuration.instance_number = msg.getInt(0);
  sprintf(configuration.packet_header_string, "%s%d\0", PacketHeaderString, configuration.instance_number);
  
  #if DEBUG == 1
    Serial.print("new address header: ");
    Serial.println(configuration.packet_header_string);
  #endif
  
  flash_config.write(configuration);
}


// Update device's communication port
void set_port(OSCMessage &msg) 
{
  #if DEBUG == 1
    Serial.print("Port changed from ");
    Serial.print(configuration.localPort);
  #endif

  // Get new port, stop listening on old port, start on new port
  configuration.localPort = msg.getInt(0);
  Udp.stop();
  Udp.begin(configuration.localPort);

  #if DEBUG == 1
    Serial.print(" to ");
    Serial.println(configuration.localPort);
  #endif

  // Update saved port in configuration
  flash_config.write(configuration);
}


// Updates WiFi ssid with new ssid 
void set_ssid(OSCMessage &msg) 
{
  msg.getString(0, new_ssid, 50);
  ssid_set = true;
}


// Updates WiFi password with new password 
void set_pass(OSCMessage &msg) 
{
  msg.getString(0, new_pass, 50);
  pass_set = true;
}


// Broadcast IP address so that requesting computer can update IP
// to send to if it only had device instance number
void broadcastIP(OSCMessage &msg) {
  configuration.ip = WiFi.localIP();
  char addressString[255];
  OSCBundle bndl;

  sprintf(addressString, "%s%s", configuration.packet_header_string, "/NewIP");
  bndl.add(addressString).add((int32_t)configuration.ip[0])
                         .add((int32_t)configuration.ip[1])
                         .add((int32_t)configuration.ip[2])
                         .add((int32_t)configuration.ip[3]);

  Udp.beginPacket(configuration.ip_broadcast, configuration.localPort);
  bndl.send(Udp);     // Send the bytes to the SLIP stream
  Udp.endPacket();    // Mark the end of the OSC Packet
  bndl.empty();       // Empty the bundle to free room for a new one

  #if DEBUG == 1
    Serial.print("Broadcasted IP: ");
    Serial.println(configuration.ip);
  #endif
}


// Used to route messagse to the correct function
void msg_router(OSCMessage &msg, int addrOffset) {
  #if DEBUG == 1
    char buffer[100];
    msg.getAddress(buffer, addrOffset);
    Serial.print("Parsed ");
    Serial.println(buffer);
  #endif
  #ifdef is_servo
    msg.dispatch("/Servo/Set", set_servo, addrOffset);
  #endif
  #ifdef is_relay
    msg.dispatch("/Relay/State", handleRelay, addrOffset);
  #endif
  #ifdef is_mpu6050
    msg.dispatch("/MPU6050/cal", calMPU6050_OSC, addrOffset);
  #endif
  #ifdef is_neopixel
    msg.dispatch("/Neopixel", setColor, addrOffset);
  #endif

  msg.dispatch("/Connect/SSID", set_ssid, addrOffset);
  msg.dispatch("/Connect/Password", set_pass, addrOffset);
  
  msg.dispatch("/wifiSetup/AP", switch_to_AP, addrOffset);
  msg.dispatch("/SetID", set_instance_num, addrOffset);
  msg.dispatch("/SetPort", set_port, addrOffset);
  
  msg.dispatch("/requestIP", broadcastIP, addrOffset);
}






void init_config()
{
  #if MEM_TYPE == MEM_FLASH
    configuration = flash_config.read();                    // Read from flash memory
    
    #if DEBUG == 1
      Serial.println("Reading from flash.");
      Serial.print("Checksum: ");
      Serial.println(configuration.checksum);
    #endif
    
    if (configuration.checksum != memValidationValue) {     // Write default values to flash
          configuration.instance_number = INIT_INST;
          sprintf(configuration.packet_header_string,"%s%d\0",PacketHeaderString,configuration.instance_number);
          #if DEBUG == 1
            Serial.print("expecting OSC header ");
            Serial.println(configuration.packet_header_string);
          #endif
          configuration.my_ssid = AP_NAME;                  // Default AP name
          strcpy(configuration.ssid,DEFAULT_NETWORK);       // Created AP name
          strcpy(configuration.pass,DEFAULT_PASSWORD);      // AP password (needed only for WEP, must be exactly 10 or 26 characters in length)
          configuration.keyIndex = 0;                       // Your network key Index number (needed only for WEP)
          configuration.ip_broadcast = "192.168.1.255";     // IP to Broadcast data 
          configuration.localPort = INIT_PORT;              // Local port to listen on
          configuration.wifi_mode = DEFAULT_MODE;           // WiFi mode to start in (AP_MODE, WPA_CLIENT_MODE, WEP_CLIENT_MODE)
          
          // Add any other behavior/calibration wrapped in an #ifdef is_something preprocessor directive HERE
          
          #ifdef is_mpu6050
            calMPU6050();                                   // Calibration writes memValidationValue for us
          #else
            configuration.checksum = memValidationValue;    // Configuration has been written successfully, so we write the checksum
          #endif
          
          #if DEBUG == 1
            Serial.println("Writing to flash for the first time.");
          #endif
          
          flash_config.write(configuration);                // Don't uncomment this line until we're pretty confident that this behaves how we want; 
                                                            // Flash memory has limited writes and we don't want to waste it on unnecessary tests
    }
    #if DEBUG == 1     //If the read from memory is successful.
    else { 
      // Print out the files read from flash
      Serial.print("OSC address header: ");
      Serial.println(configuration.packet_header_string);
      Serial.print("AP SSID: ");
      Serial.println(configuration.my_ssid);
      Serial.print("SSID: ");
      Serial.println(configuration.ssid);
      Serial.print("Pass: ");
      Serial.println(configuration.pass);
      Serial.print("Key Index: ");
      Serial.println(configuration.keyIndex);
      Serial.print("IP to Broadcast to: ");
      Serial.println(configuration.ip_broadcast);
      Serial.print("Local Port: ");
      Serial.println(configuration.localPort);
      // Add any other debug outputs here, wrapped in a preprocessor #ifdef is_something directive
    }
    #endif // of DEBUG
  #endif // of MEM_TYPE
}


#ifdef transmit_butt
void check_button_held()
{
  if ( (uint32_t)digitalRead(transmit_butt) ){
      button_timer = 0;
    } else {
      #ifdef is_sleep_period
        button_timer += is_sleep_period;
      #else
        button_timer++;
      #endif
      if (button_timer >= 5000) { // ~about 5 seconds
        #if DEBUG == 1
          Serial.println("Button held for 8 seconds, resetting to AP mode");
        #endif
        button_timer = 0;
     
        OSCMessage temp;          // Not used by function, but it expects an OSCMessage normally
        switch_to_AP(temp);       // Change to AP mode
      }
    }
}
#endif




// Given a string, replace all instances of 'orig' char with 'rep' char
// Used primarily for replacing '~'s sent by Max, as it cannot send strings with spaces
void replace_char(char *str, char orig, char rep) {
    char *ix = str;
    while((ix = strchr(ix, orig)) != NULL) {
        *ix++ = rep;
    }
}
// Previous non-generic solution - delete once the above is verified      
//        int i = 0;
//        while (new_ssid[i] != '\0'){
//          if (new_ssid[i] == '~')
//            new_ssid[i] = ' ';
//          i++;
//        }
//        i = 0;
//        while (new_pass[i] != '\0'){
//          if (new_pass[i] == '~')
//            new_pass[i] = ' ';
//          i++;
//        }

