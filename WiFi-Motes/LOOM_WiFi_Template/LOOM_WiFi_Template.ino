/*
  January 19, 2018. Author: Trevor Swope
  This is the LOOM WiFi Mote template! Read this big 'ol comment for instructions on how to use the preprocessor statements in here to define the functionality of your board.
 
  DEBUG MODE: define DEBUG as 1 or 0. If DEBUG is set, make sure the serial monitor is open; all serial calls should be wrapped in an #if DEBUG == 1 ... #endif

  
 */

 
// ================================================================ 
// ===    INCLUDE DECLARATIONS, STRUCTS, AND FUNCTIONS FROM     === 
// ===            OTHER FILES AS SET IN CONFIG.H                ===
// ================================================================ 

// Config has to be first has it hold all user specified options
#include "config.h"

// Preamble includes any relevant subroutine files based on options
// specified in the above config
#include "loom_preamble.h"


// ================================================================ 
// ===                           SETUP                          ===
// ================================================================ 
void setup() {
  pinMode(led, OUTPUT);   // Set the LED pin mode
  LOOM_begin();           // LOOM_begin calls any relevant (based on config) LOOM device setup functions
}

// ================================================================ 
// ===                        MAIN LOOP                         ===
// ================================================================ 
void loop() {
  OSCBundle bndl; 

	#if is_tca9548a == 1
		measure_tca9548a();
	#endif
	


  // Reset to AP mode if button held for ~5 seconds
  #if defined(button) && (is_wifi == 1)
    check_button_held();      
  #endif

  #if is_lora == 1
    lora_receive_bundle(&bndl);                      // Not tested yet - code commented out below is in loom_lora.h
  #endif

//  #if is_lora == 1
//    #if lora_device_type == 0
//      if (manager.available()) {
//        uint8_t len = LORA_MESSAGE_SIZE;
//        uint8_t from;
//        uint8_t buf[LORA_MESSAGE_SIZE];
//        memset(buf, '\0', LORA_MESSAGE_SIZE);
//        if (manager.recvfromAck(buf, &len, &from)) {
//          if (((char)(buf[0])) == '/') {
//            get_OSC_bundle((char*)buf, &bndl); 
//            for(int i = 0; i < NUM_FIELDS; i++)
//              data[i] = get_data_value(bndl.getOSCMessage(0), i);
//          } else {
//            char str[LORA_MESSAGE_SIZE];
//            String((char*)buf).toCharArray(str, sizeof(str)-1);
//            char *token;
//            char *savept = str;
//            String cols[8] = {"IDtag", "RTC_time", "temp", "humidity", "loadCell", "vbat"};
//            for(int i = 0; i < NUM_FIELDS; i+=2) {
//              token = strtok_r(savept, ",", &savept);
//              if(token != NULL) {
//                data[i] = cols[i/2];
//                data[i+1] = String(token);
//              }
//            } // of for
//          } // of else 
//          #if DEBUG == 1
//            print_bundle(&bndl);
//          #endif
//          sendToPushingBox(int(NUM_FIELDS), server_name, device_id);
//        } // of if (manager.recvfromAck(buf, &len, &from))
//      } // of if (manager.available()) 
//    #endif //of lora type is hub
//
//    #if lora_device_type == 1
//      bndl.add(PacketHeaderString).add("Date").add("3/6/2018").add("IDtag").add((int32_t) INIT_INST)
//      .add("TimeStamp").add("2018").add("TempC").add((int32_t)32).add("Humidity").add((float)46.4)
//      .add("LoadCell").add((int32_t)1000).add("IRLight").add((int32_t)2000).add("FullLight").add((int32_t)3000)
//      .add("BatVolt").add((float)4.2);
//    
//      char message[RH_RF95_MAX_MESSAGE_LEN];
//      memset(message, '\0', sizeof(message));
//      get_OSC_string(&bndl, message);
//    
//      #if DEBUG == 1
//        Serial.println(message);
//        Serial.print("Message length: ");
//        Serial.println(strlen(message));
//        Serial.print("Max message length: ");
//        Serial.println(RH_RF95_MAX_MESSAGE_LEN);
//        Serial.print("Sending...");
//      #endif
//       
//      if (manager.sendtoWait((uint8_t*)message, strlen(message), SERVER_ADDRESS)){
//         #if DEBUG == 1
//           Serial.println("ok");
//         #endif 
//      }
//      else {
//        #if DEBUG == 1
//          Serial.println("failed");
//        #endif
//      }
//    
//      delay(10000);
//    #endif // of lora type is_node
//  #endif // of is_lora 




  // HANDLE WIFI BUNDLE
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

  #if is_wifi == 1
//    state_wifi->pass_set = state_wifi->ssid_set = false;
    wifi_receive_bundle(&bndl, configuration.packet_header_string);                             // NOT tested yet - function is currently in loom_common, should probably be moved to wifi file, but it requires msg_router defined in loom_common
  #endif



//  #if is_wifi == 1
//    int packetSize;
//    state_wifi->pass_set = state_wifi->ssid_set = false;
//  #endif
//
//  #if is_wifi == 1
//  // If there's data available, read a packet
//  packetSize = Udp.parsePacket();
//  if (packetSize > 0) {
//    #if DEBUG == 1
//      Serial.println("=========================================");
//      Serial.print("Received packet of size: ");
//      Serial.println(packetSize);
//    #endif
//    
//    bndl.empty();             // Empty previous bundle
//    while (packetSize--){     // Read in new bundle
//      bndl.fill(Udp.read());
//    }
//
//    // If no error
//    if (!bndl.hasError()){
//      #if DEBUG == 1
//        Serial.print("Number of items in bundle: ");
//        Serial.println(bndl.size());
//        Serial.print("First message address string: ");
//        bndl.getOSCMessage(0)->getAddress(addressString, 0);
//        Serial.println(addressString);
//      #endif
//        
//      for (int i = 0; i < 32; i++){ //Clear the new_ssid and new_pass buffers
//        state_wifi->new_ssid[i] = '\0';
//        state_wifi->new_pass[i] = '\0';
//      }
//
//      // Send the bndle to the routing function, which will route/dispatch messages to the currect handling functions
//      // Most commands will be finished once control returns here (WiFi changes being handled below)
//      bndl.route(configuration.packet_header_string,msg_router);
//
//      // Write on/off values to connected relays
//      #if is_relay == 1
//        write_relay_states();       
//      #endif
//      
//      // If new ssid and password have been received, try to connect to that network
//      if (state_wifi->ssid_set == true && state_wifi->pass_set == true){
//        connect_to_new_network();   
//      }
//
//    } else { // of !bndl.hasError()
//      error = bndl.getError();
//      #if DEBUG == 1
//        Serial.print("error: ");
//        Serial.println(error);
//      #endif
//    } // of else
//  } // of (packetSize > 0)
//
//  #endif //is_wifi
//  
//  // END OF HANDLE WIFI BUNDLE
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

                           // Currently commented out as write_relay_states is called at end of handle_relay after bundle to change state
                           // Uncomment if this needs to be set each loop iteration
  // Write on/off values to connected relays
//  #if is_relay == 1
//    write_relay_states();                              
//  #endif


  #if is_wifi == 1
		// Compare the previous status to the current status
		if (status != WiFi.status()) {
			status = WiFi.status();              // It has changed, update the variable
			
			#if DEBUG == 1
				if (status == WL_AP_CONNECTED) {   // A device has connected to the AP
						print_remote_mac_addr();                            
				} else {                           // A device has disconnected from the AP, and we are back in listening mode 
					Serial.println("Device disconnected from AP");
				}
			#endif
		} // of if ( status != WiFi.status() )


		//BEGIN SENDING OF DATA
		OSCBundle send_bndl;
		send_bndl.empty();
		// Measure battery voltage
		vbat = analogRead(VBATPIN);
		vbat = (vbat * 2 * 3.3) / 1024; // We divided by 2, so multiply back, multiply by 3.3V, our reference voltage, div by 1024 to convert to voltage

		sprintf(addressString, "%s%s", configuration.packet_header_string, "/vbat");
		send_bndl.add(addressString).add(vbat);          // Tack battery voltage onto here. Will want to change this for other sensors
  #endif // of if is_wifi == 1



  #if is_wifi == 1
		// Update MPU6050 Data
		#if is_mpu6050 == 1
			measure_mpu6050();      // Now measure MPU6050, update values in global registers 
			package_mpu6050(&send_bndl,configuration.packet_header_string);                // Build and send packet
			// UDP Packet
			Udp.beginPacket(configuration.config_wifi.ip_broadcast, configuration.config_wifi.localPort);
			send_bndl.send(Udp);    // Send the bytes to the SLIP stream
			Udp.endPacket();        // Mark the end of the OSC Packet
			send_bndl.empty();      // Empty the bundle to free room for a new one
			mpu.resetFIFO();        // Flush MPU6050 FIFO to avoid overflows if using i2c
		#endif
    #if is_max31856 == 1
      measure_max31856();
      package_max31856(&send_bndl,configuration.packet_header_string);
      Udp.beginPacket(configuration.config_wifi.ip_broadcast, configuration.config_wifi.localPort);
      send_bndl.send(Udp);    // Send the bytes to the SLIP stream
      Udp.endPacket();        // Mark the end of the OSC Packet
      send_bndl.empty();      // Empty the bundle to free room for a new one
    #endif
		// Get analog readings
		#if is_analog >= 1
			package_analog(&send_bndl,configuration.packet_header_string);
			Udp.beginPacket(configuration.config_wifi.ip_broadcast, configuration.config_wifi.localPort);
			send_bndl.send(Udp);    // Send the bytes to the SLIP stream
			Udp.endPacket();        // Mark the end of the OSC Packet
			send_bndl.empty();      // Empty the bundle to free room for a new one
		#endif
	#endif // of is_wifi



  // Delay between loop iterations
  #ifdef is_sleep_period
    loop_sleep();
  #endif
  
} // End loop section


