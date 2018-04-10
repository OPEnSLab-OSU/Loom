/*
  January 19, 2018. Author: Trevor Swope
  This is the LOOM WiFi Mote template! Read this big 'ol comment for instructions on how to use the preprocessor statements in here to define the functionality of your board.
 
  DEBUG MODE: define DEBUG as 1 or 0. If DEBUG is set, make sure the serial monitor is open; all serial calls should be wrapped in an #if DEBUG == 1 ... #endif

  //ADD MORE DESCRIPTION HERE
  
 */
// -------------------------------------------------------------
// INCLUDE DECLARATIONS, STRUCTS, AND FUNCTIONS FROM OTHER FILES
// -------------------------------------------------------------

// Config has to be first
#include "config.h"
#include "preamble.h"



// -------------------------------------------------------------
//                            SETUP 
// -------------------------------------------------------------
void setup() {

  #ifdef transmit_butt
    pinMode(transmit_butt, INPUT_PULLUP); // Set the transmit_butt pin mode to input
  #endif

  pinMode(led, OUTPUT);                   // Set the LED pin mode

  // Actuator-specific setups
  #ifdef is_neopixel
    neopixel_setup();
  #endif
  #ifdef is_servo
    servo_setup();
  #endif
  #ifdef is_relay
    relay_setup();
  #endif
  
  //Initialize serial and wait for port to open:
  #if DEBUG == 1
    Serial.begin(9600);
    while(!Serial);       ..3333   // Ensure Serial is ready to go before anything happens in DEBUG mode.
  #endif
 
  #ifdef is_i2c
    i2c_setup();
  #endif

  init_config();
  
  #ifdef is_wifi
    wifi_setup();
  #endif

  #ifdef is_lora
    lora_setup(&rf95, &manager);
  #endif
  
} // end of setup()



// -------------------------------------------------------------
//                          MAIN LOOP
// -------------------------------------------------------------
void loop() { 
  // Loop variables and structures
  OSCBundle bndl;
  char addressString[255];
  
  #ifdef is_wifi
    pass_set = ssid_set = false;
    int packetSize;
  #endif

  // Reset to AP mode if button held for ~5 seconds
  #if defined(transmit_butt) && defined(is_wifi)
    check_button_held();      
  #endif



  #ifdef is_lora
    if (manager.available()) {
      uint8_t len = LORA_MESSAGE_SIZE;
      uint8_t from;
      uint8_t buf[LORA_MESSAGE_SIZE];
      memset(buf, '\0', LORA_MESSAGE_SIZE);
      if (manager.recvfromAck(buf, &len, &from)) {
        if (((char)(buf[0])) == '/') {
          get_OSC_bundle((char*)buf, &bndl); 
          for(int i = 0; i < NUM_FIELDS; i++)
            data[i] = get_data_value(bndl.getOSCMessage(0), i);
        } else {
          char str[LORA_MESSAGE_SIZE];
          String((char*)buf).toCharArray(str, sizeof(str)-1);
          char *token;
          char *savept = str;
          String cols[8] = {"IDtag", "RTC_time", "temp", "humidity", "loadCell", "lightIR", "lightFull", "vbat"};
          for(int i = 0; i < NUM_FIELDS; i+=2) {
            token = strtok_r(savept, ",", &savept);
            if(token != NULL) {
              data[i] = cols[i/2];
              data[i+1] = String(token);
            }
          }
        } 
        sendToPushingBox(int(NUM_FIELDS), server_name, device_id);
        #if DEBUG == 1
          print_bundle(&bndl);
        #endif
      }
    }
  #endif // is_lora



  // HANDLE BUNDLE
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

  #ifdef is_wifi
  // If there's data available, read a packet
  packetSize = Udp.parsePacket();
  if (packetSize > 0) {
    #if DEBUG == 1
      Serial.println("=========================================");
      Serial.print("Received packet of size: ");
      Serial.println(packetSize);
    #endif
    
    bndl.empty();             // Empty previous bundle
    while (packetSize--){     // Read in new bundle
      bndl.fill(Udp.read());
    }

    // If no error
    if (!bndl.hasError()){
      #if DEBUG == 1
        Serial.print("Number of items in bundle: ");
        Serial.println(bndl.size());
        Serial.print("First message address string: ");
        bndl.getOSCMessage(0)->getAddress(addressString, 0);
        Serial.println(addressString);
      #endif
        
      for (int i = 0; i < 32; i++){ //Clear the new_ssid and new_pass buffers
        new_ssid[i] = '\0';
        new_pass[i] = '\0';
      }

      // Send the bndle to the routing function, which will route/dispatch messages to the currect handling functions
      // Most commands will be finished once control returns here (WiFi changes being handled below)
      bndl.route(configuration.packet_header_string,msg_router);

      // Write on/off values to connected relays
      #ifdef is_relay
        write_relay_states();       
      #endif
      
      // If new ssid and password have been received, try to connect to that network
      if (ssid_set == true && pass_set == true){
        connect_to_new_network();   
      }

    } else { // of !bndl.hasError()
      error = bndl.getError();
      #if DEBUG == 1
        Serial.print("error: ");
        Serial.println(error);
      #endif
    } // of else
  } // of (packetSize > 0)

  #endif //is_wifi
  
  // END OF HANDLE BUNDLE
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


  #ifdef is_wifi
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
  #endif

  // Measure battery voltage
  vbat = analogRead(VBATPIN);
  vbat = (vbat * 2 * 3.3) / 1024; // We divided by 2, so multiply back, multiply by 3.3V, our reference voltage, div by 1024 to convert to voltage

  
  // Update MPU6050 Data
  #ifdef is_mpu6050
    measure_mpu6050();            // Now measure MPU6050, update values in global registers 
    udp_mpu6050();                // Build and send packet
    mpu.resetFIFO();              // Flush MPU6050 FIFO to avoid overflows if using i2c
  #endif

  // Get analog readings
  #ifdef is_analog
      measure_analog();
  #endif

  // Delay between loop iterations
  #ifdef is_sleep_period
    #if DEBUG == 0
      int sleepMS = Watchdog.sleep(is_sleep_period); // Sleep MCU for transmit period duration
    #else
      delay(is_sleep_period);                        // Demo transmit every 1 second
    #endif
  #endif
  
} // End loop section


