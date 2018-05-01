// ================================================================
// ===                 COMMON GLOBAL VARIABLES                  ===
// ================================================================
int           led = LED_BUILTIN;              // LED pin number
volatile bool ledState = LOW;                 // State of LED
float         vbat = 3.3;                     // Place to save measured battery voltage (3.3V max)
char          packetBuffer[255];              // Buffer to hold incoming packet
char          ReplyBuffer[] = "acknowledged"; // A string to send back
OSCErrorCode  error;                          // Hold errors from OSC
uint32_t      button_timer;                   // For time button has been held
char          addressString[255];


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void set_instance_num(OSCMessage &msg);
void msg_router(OSCMessage &msg, int addrOffset);
#if (is_wifi == 1) && defined(button)
  void check_button_held();
#endif
void LOOM_begin();
void loop_sleep();

// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================

// --- SET INSTANCE NUMBER ---
// Updates device's identifying instance number
// Arguments: msg (OSC message with new instance number)
// Return:    none
void set_instance_num(OSCMessage &msg) 
{
  configuration.instance_number = msg.getInt(0);
  sprintf(configuration.packet_header_string, "%s%d\0", PacketHeaderString, configuration.instance_number);
  
  #if LOOM_DEBUG == 1
    Serial.print("new address header: ");
    Serial.println(configuration.packet_header_string);
  #endif
  write_non_volatile();
  //flash_setup.write(configuration);
}
 
// --- MESSAGE ROUTER ---
// Used to route OSC messages to the correct function to handle it
// Arguments: msg (OSC message to route subroutine that handles it), 
//   addrOffset (used to determine where to start check of message header string)
// Return:    none
void msg_router(OSCMessage &msg, int addrOffset) {
  #if LOOM_DEBUG == 1
    char buffer[100];
    msg.getAddress(buffer, addrOffset);
    Serial.print("Parsed ");
    Serial.println(buffer);
  #endif

  #if is_tca9548a
    if (msg.fullMatch("/GetSensors", addrOffset)){
      msg.add(configuration.packet_header_string);
      #if LOOM_DEBUG == 1
        Serial.println("Got a request for sensor list");
      #endif
    }
    
    msg.dispatch("/GetSensors",   send_sensor_list, addrOffset);
  #endif
  #if num_servos > 0  
    msg.dispatch("/Servo/Set",    set_servo,      addrOffset);
  #endif
  #if is_relay == 1
    msg.dispatch("/Relay/State",  handleRelay,    addrOffset);
  #endif
  #if is_mpu6050 == 1
    msg.dispatch("/MPU6050/cal",  calMPU6050_OSC, addrOffset);
  #endif
  #if is_neopixel == 1
    msg.dispatch("/Neopixel",     setColor,       addrOffset);
  #endif
	#if is_lora == 1 && lora_device_type == 1
		msg.dispatch("/SendToPB", 				
  #endif
	
  #if is_wifi == 1
    msg.dispatch("/Connect/SSID",     set_ssid,     addrOffset);
    msg.dispatch("/Connect/Password", set_pass,     addrOffset);
    msg.dispatch("/wifiSetup/AP",     switch_to_AP, addrOffset);
    msg.dispatch("/SetPort",          set_port,     addrOffset);
    msg.dispatch("/requestIP",        broadcastIP,  addrOffset);
    msg.dispatch("/getNewChannel",    new_channel,  addrOffset);
  #endif
	

	
  msg.dispatch("/SetID", set_instance_num, addrOffset);
}


#if is_wifi == 1
	// --- CHECK BUTTON HELD ---
	// Checked each iteration of main loop if the device's button has been held
	// If so, restart in access point mode
	// Arguments: none
	// Return:    none
	#ifdef button
	void check_button_held()
	{
		if ( (uint32_t)digitalRead(button) ) {
			button_timer = 0;
		} 
		else {
			#ifdef is_sleep_period
				button_timer += is_sleep_period;
			#else
				button_timer++;
			#endif
			if (button_timer >= 5000) { // ~about 8 seconds
				#if LOOM_DEBUG == 1
					Serial.println("Button held for 8 seconds, resetting to AP mode");
				#endif
				button_timer = 0;
		 
				OSCMessage temp;          // Dummy message not used by function, but it expects an OSCMessage normally
				switch_to_AP(temp);       // Change to AP mode
			} 
		} // of else 
	}
	#endif // of ifdef button
#endif //is_wifi == 1


// ================================================================
// ===                        LOOM BEGIN                        ===
// ================================================================
// Called by setup(), handles calling of any LOOM specific individual device setups
// Starts Wifi or Lora and serial if debugging prints are on
// Arguments: none
// Return:    none 
void LOOM_begin()
{
  //Initialize serial and wait for port to open:
  #if LOOM_DEBUG == 1
    Serial.begin(9600);
    while(!Serial);        // Ensure Serial is ready to go before anything happens in LOOM_DEBUG mode.
		delay(5000);
		Serial.println("Initialized Serial!");
  #endif
  
  // Set the button pin mode to input
  #ifdef button
    pinMode(button, INPUT_PULLUP); 
  #endif
	
	#if is_tca9548a == 1
		setup_tca9548a();
	#endif
  #if is_decagon == 1
    deca_gs3_setup(); //rename this
  #endif
  #if is_mpu6050 > 0
    setup_mpu6050();
  #endif 
  #ifdef is_max31856
    setup_max31856();
  #endif 
  #if is_neopixel == 1
    setup_neopixel();
  #endif
  #if num_servos > 0
    setup_servo();
  #endif
  #if is_relay == 1
    setup_relay();
  #endif

  flash_config_setup();

  // Communication Platform specific setups
  #if is_wifi == 1
    wifi_setup();
  #endif
  #if is_lora == 1
    lora_setup(&rf95, &manager);
  #endif
	
  
}





#if is_wifi == 1
void wifi_receive_bundle(OSCBundle *bndl, char packet_header_string[], WiFiUDP *Udp, unsigned int port)
{  
  int packetSize; 
  state_wifi.pass_set = false;
  state_wifi.ssid_set = false;
  
  // If there's data available, read a packet
  packetSize = Udp->parsePacket();

  if (packetSize > 0) {
    #if LOOM_DEBUG == 1
      if (packetSize > 0){
          Serial.println("=========================================");
          Serial.print("Received packet of size: ");
          Serial.print(packetSize);
          Serial.print(" on port " );
          Serial.println(port);
      }
    #endif
    
    bndl->empty();             // Empty previous bundle
    while (packetSize--){     // Read in new bundle
      bndl->fill(Udp->read());
    }

    // If no error
    if (!bndl->hasError()){
      char addressString[255];
      bndl->getOSCMessage(0)->getAddress(addressString, 0);

      #if LOOM_DEBUG == 1
        Serial.print("Number of items in bundle: ");
        Serial.println(bndl->size());
        Serial.print("First message address string: ");
        Serial.println(addressString);
      #endif

      if (strcmp(addressString, "/LOOM/ChannelPoll") == 0) {
        #if LOOM_DEBUG == 1
          Serial.println("Received channel poll request");
        #endif
        response_to_poll_request(packet_header_string);
        return;
      }
      
      for (int i = 0; i < 32; i++){ //Clear the new_ssid and new_pass buffers
        state_wifi.new_ssid[i] = '\0';
        state_wifi.new_pass[i] = '\0';
      }

      
      // Send the bndle to the routing function, which will route/dispatch messages to the currect handling functions
      // Most commands will be finished once control returns here (WiFi changes being handled below)
      bndl->route(packet_header_string,msg_router);
      
      // If new ssid and password have been received, try to connect to that network
      if (state_wifi.ssid_set == true && state_wifi.pass_set == true){
        connect_to_new_network();   
      }

    } else { // of !bndl.hasError()
      error = bndl->getError();
      #if LOOM_DEBUG == 1
        Serial.print("error: ");
        Serial.println(error);
      #endif
    } // of else
  } // of (packetSize > 0)
}
#endif // of if is_wifi == 1




#ifdef is_sleep_period
void loop_sleep()
{
  #if LOOM_DEBUG == 0
    int sleepMS = Watchdog.sleep(is_sleep_period); // Sleep MCU for transmit period duration
  #else
    delay(is_sleep_period);                        // Demo transmit every 1 second
  #endif
}
#endif


