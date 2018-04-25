/*
  January 19, 2018. Author: Trevor Swope
  This is the LOOM WiFi Mote template! Read this big 'ol comment for instructions on how to use the preprocessor statements in here to define the functionality of your board.
 
  LOOM_DEBUG MODE: define LOOM_DEBUG as 1 or 0. If LOOM_DEBUG is set, make sure the serial monitor is open; all serial calls should be wrapped in an #if LOOM_DEBUG == 1 ... #endif

  
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
    lora_receive_bundle(&bndl);                      // Not tested yet - code that was here is now in loom_lora.h under func lora_receive_bundle
  #endif

  #if is_wifi == 1
    // Handle wifi bundle if it exists
    wifi_receive_bundle(&bndl, configuration.packet_header_string, &Udp, configuration.config_wifi.localPort); 
    wifi_receive_bundle(&bndl, configuration.packet_header_string, &UdpCommon, configuration.config_wifi.commonPort);                             
                           
    // Compare the previous status to the current status
    wifi_check_status();

    
		//BEGIN SENDING OF DATA
		OSCBundle send_bndl;
		send_bndl.empty();
		// Measure battery voltage
		vbat = analogRead(VBATPIN);
		vbat = (vbat * 2 * 3.3) / 1024; // We divided by 2, so multiply back, multiply by 3.3V, our reference voltage, div by 1024 to convert to voltage

		sprintf(addressString, "%s%s", configuration.packet_header_string, "/vbat");
		send_bndl.add(addressString).add(vbat);          // Tack battery voltage onto here. Will want to change this for other sensors

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


