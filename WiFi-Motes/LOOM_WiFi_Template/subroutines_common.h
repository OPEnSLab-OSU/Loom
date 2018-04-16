
extern void switch_to_AP(OSCMessage &msg);  // Reference to externally defined function called by msg_router




// --- SET INSTANCE NUMBER ---
// Updates device's identifying instance number
// Arguments: msg (OSC message with new instance number)
// Return:    none
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




// --- MESSAGE ROUTER ---
// Used to route OSC messages to the correct function to handle it
// Arguments: msg (OSC message to route subroutine that handles it), 
//   addrOffset (used to determine where to start check of message header string)
// Return:    none
void msg_router(OSCMessage &msg, int addrOffset) {
  #if DEBUG == 1
    char buffer[100];
    msg.getAddress(buffer, addrOffset);
    Serial.print("Parsed ");
    Serial.println(buffer);
  #endif
  #if num_servos > 0
    msg.dispatch("/Servo/Set", set_servo, addrOffset);
  #endif
  #if is_relay == 1
    msg.dispatch("/Relay/State", handleRelay, addrOffset);
  #endif
  #ifdef is_mpu6050
    msg.dispatch("/MPU6050/cal", calMPU6050_OSC, addrOffset);
  #endif
  #ifdef is_neopixel
    msg.dispatch("/Neopixel", setColor, addrOffset);
  #endif

  #if is_wifi == 1
    msg.dispatch("/Connect/SSID", set_ssid, addrOffset);
    msg.dispatch("/Connect/Password", set_pass, addrOffset);
    msg.dispatch("/wifiSetup/AP", switch_to_AP, addrOffset);
    msg.dispatch("/SetPort", set_port, addrOffset);
    msg.dispatch("/requestIP", broadcastIP, addrOffset);
  #endif
  msg.dispatch("/SetID", set_instance_num, addrOffset);
}



// --- INITIAL CONFIG ---
// Handles configuration of reading device config from flash (if applicable)
//   else uses config.h specific settings, and writes these to flash
// Arguments: none
// Return:    none
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
          #if is_wifi == 1
            configuration.my_ssid = AP_NAME;                  // Default AP name
            strcpy(configuration.ssid,DEFAULT_NETWORK);       // Default network name
            strcpy(configuration.pass,DEFAULT_PASSWORD);      // AP password (needed only for WEP, must be exactly 10 or 26 characters in length)
            configuration.keyIndex = 0;                       // Your network key Index number (needed only for WEP)
            configuration.ip_broadcast = "192.168.1.255";     // IP to Broadcast data 
            configuration.localPort = INIT_PORT;              // Local port to listen on
            configuration.wifi_mode = DEFAULT_MODE;           // WiFi mode to start in (AP_MODE, WPA_CLIENT_MODE, WEP_CLIENT_MODE)
          #endif
          // Add any other behavior/calibration wrapped in an #ifdef is_something preprocessor directive HERE
          
          #ifdef is_mpu6050
            calMPU6050();                                     // Calibration writes memValidationValue for us
          #else
            configuration.checksum = memValidationValue;      // Configuration has been written successfully, so we write the checksum
          #endif

          #if DEBUG == 1
            Serial.println("Writing to flash for the first time.");
          #endif
          
          flash_config.write(configuration);                  // Don't uncomment this line until we're pretty confident that this behaves how we want; 
                                                              // Flash memory has limited writes and we don't want to waste it on unnecessary tests
    } // of if (configuration.checksum != memValidationValue)
  #endif // of MEM_TYPE
}



// --- CHECK BUTTON HELD ---
// Checked each iteration of main loop if the device's button has been held
// If so, restart in access point mode
// Arguments: none
// Return:    none
#ifdef button
void check_button_held()
{
  if ( (uint32_t)digitalRead(button) ){
  button_timer = 0;
  } else {
    #ifdef is_sleep_period
      button_timer += is_sleep_period;
    #else
      button_timer++;
    #endif
    if (button_timer >= 5000) { // ~about 8 seconds
      #if DEBUG == 1
        Serial.println("Button held for 8 seconds, resetting to AP mode");
      #endif
      button_timer = 0;
   
      OSCMessage temp;          // Dummy message not used by function, but it expects an OSCMessage normally
      switch_to_AP(temp);       // Change to AP mode
    } 
  } // of else 
}
#endif



// --- LOOM BEGIN ---
// Called by setup(), handles calling of any LOOM specific individual device setups
// Starts Wifi or Lora and serial if debugging prints are on
// Arguments: none
// Return:    none 
void LOOM_begin()
{
  //Initialize serial and wait for port to open:
  #if DEBUG == 1
    Serial.begin(9600);
    while(!Serial);        // Ensure Serial is ready to go before anything happens in DEBUG mode.
  #endif
  
  // Set the button pin mode to input
  #ifdef button
    pinMode(button, INPUT_PULLUP); 
  #endif

  #ifdef is_i2c
    i2c_setup();
  #endif


  // Primary configuratation, such as writing config to non-volatile memory
  init_config();


  // Actuator-specific setups
  #ifdef is_neopixel
    neopixel_setup();
  #endif
  #if num_servos > 0
    servo_setup();
  #endif
  #if is_relay == 1
    relay_setup();
  #endif
  
 
  
  #if is_wifi == 1
    wifi_setup();
  #endif

  #if is_lora == 1
    lora_setup(&rf95, &manager);
  #endif
  
}



