/*
  January 19, 2018. Author: Trevor Swope
  This is the LOOM WiFi Mote template! Read this big 'ol comment for instructions on how to use the preprocessor statements in here to define the functionality of your board.
 
  DEBUG MODE: define DEBUG as 1 or 0. If DEBUG is set, make sure the serial monitor is open; all serial calls should be wrapped in an #if DEBUG == 1 ... #endif

  //ADD MORE DESCRIPTION HERE
  
 */

// Necessary Includes
#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>


// Needed by config.h
enum WiFiMode{
  AP_MODE,
  WPA_CLIENT_MODE,
  WEP_CLIENT_MODE
};


// -------------------------------------------------------------
// INCLUDE DECLARATIONS, STRUCTS, AND FUNCTIONS FROM OTHER FILES
// -------------------------------------------------------------

// Config has to be first
#include "config.h"
#include "declarations_and_structures.h"

// Using Config defines, check which headers to include
#ifdef is_analog
  #include "analogIn_subroutines.h"
#endif
#ifdef is_i2c
  #include "mpu6050_subroutines.h"
#endif
#ifdef is_neopixel
  #include "neopixel_subroutines.h"
#endif
#ifdef is_servo  
  #include "servo_subroutines.h"
#endif
#ifdef is_relay
  #include "relay_subroutines.h"
#endif

// These may refer to functions in above headers
#include "common_subroutines.h"
#include "wifi_subroutines.h"



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
    while(!Serial);          // Ensure Serial is ready to go before anything happens in DEBUG mode.
  #endif
 
  #ifdef is_i2c
    i2c_setup();
  #endif

  init_config();
  wifi_setup();
  
} // end of setup()



// -------------------------------------------------------------
//                          MAIN LOOP
// -------------------------------------------------------------
void loop() { 
  // Loop variables and structures 
  pass_set = ssid_set = false;
  OSCBundle bndl;
  char addressString[255];
  int packetSize;


  // Reset to AP mode if button held for ~5 seconds
  #ifdef transmit_butt
    check_button_held();      
  #endif


  // HANDLE BUNDLE
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  
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
  
  // END OF HANDLE BUNDLE
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

 
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


