
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

int ctr;
// ================================================================ 
// ===                        MAIN LOOP                         ===
// ================================================================ 
void loop() {
  OSCBundle bndl; 

  // Reset to AP mode if button held for ~5 seconds
  #if defined(button) && (is_wifi == 1)
    check_button_held();      
  #endif

  #if is_lora == 1
    #if lora_device_type == 0
      lora_receive_bundle(&bndl);
			if(bndl.size() > 0) {
				lora_process_bundle(&bndl, configuration.packet_header_string);
			}
		#elif lora_device_type == 2
      //TODO: repeater functionality here
    #endif
  #endif

  #if is_wifi == 1
    // Handle wifi bundle if it exists
    wifi_receive_bundle(&bndl, configuration.packet_header_string, &Udp, configuration.config_wifi.localPort); 
    wifi_receive_bundle(&bndl, configuration.packet_header_string, &UdpCommon, configuration.config_wifi.commonPort);                             
                           
    // Compare the previous status to the current status
    wifi_check_status();
  #endif
  #if is_lora == 1
  #endif

    
	//BEGIN SENDING OF DATA
	OSCBundle send_bndl;
	send_bndl.empty();

  
	// Measure battery voltage
	vbat = analogRead(VBATPIN);
	vbat = (vbat * 2 * 3.3) / 1024; // We divided by 2, so multiply back, multiply by 3.3V, our reference voltage, div by 1024 to convert to voltage
  
	sprintf(addressString, "%s%s", configuration.packet_header_string, "/vbat");
	send_bndl.add(addressString).add(vbat);          // Tack battery voltage onto here. Will want to change this for other sensors

  #ifdef button
    sprintf(addressString, "%s%s", configuration.packet_header_string, "/button");
    send_bndl.add(addressString).add((int32_t)digitalRead(button));
  #endif

  #if is_wifi == 1
    wifi_send_bundle(&send_bndl);
  #endif
  #if is_lora == 1 && lora_device_type == 1
    lora_send_bundle(&send_bndl);
  #endif
  send_bndl.empty();      // Empty the bundle to free room for a new one


  #if is_tca9548a == 1
    ctr++;
    if (ctr > 10){
      update_sensors();
      ctr = 0;
    }
    package_tca9548a(&send_bndl,configuration.packet_header_string);
    
    #if is_wifi == 1
      wifi_send_bundle(&send_bndl);
    #endif
    #if is_lora == 1 && lora_device_type == 1
      #if LOOM_DEBUG == 1
        Serial.println("Sending mux bundle...");
      #endif
      lora_send_bundle(&send_bndl);
    #endif
    send_bndl.empty();
  #endif
  
	// Update MPU6050 Data
  #if is_mpu6050 == 1
    measure_mpu6050();      // Now measure MPU6050, update values in global registers 
    package_mpu6050(&send_bndl,configuration.packet_header_string);                // Build and send packet
    #if is_wifi == 1
      wifi_send_bundle(&send_bndl);
    #endif
    #if is_lora == 1 && lora_device_type == 1
      lora_send_bundle(&send_bndl);
    #endif
    send_bndl.empty();      // Empty the bundle to free room for a new one
    mpu.resetFIFO();        // Flush MPU6050 FIFO to avoid overflows if using i2c
  #endif
  #if is_max31856 == 1
    measure_max31856();
    package_max31856(&send_bndl,configuration.packet_header_string);
    #if is_wifi == 1
      wifi_send_bundle(&send_bndl);
    #endif
    #if is_lora == 1 && lora_device_type == 1
      lora_send_bundle(&send_bndl);
    #endif
    send_bndl.empty();      // Empty the bundle to free room for a new one
  #endif
  
  // Get analog readings
  #if is_analog >= 1
    package_analog(&send_bndl,configuration.packet_header_string);
    #if is_wifi == 1
      wifi_send_bundle(&send_bndl);
    #endif
    #if is_lora == 1 && lora_device_type == 1
      lora_send_bundle(&send_bndl);
    #endif
    send_bndl.empty();      // Empty the bundle to free room for a new one
  #endif
  #if is_decagon == 1
    package_decagon(&send_bndl,configuration.packet_header_string);
    #if is_wifi == 1
      wifi_send_bundle(&send_bndl);
    #endif
    #if is_lora == 1 && lora_device_type == 1
      lora_send_bundle(&send_bndl);
    #endif
    send_bndl.empty();      // Empty the bundle to free room for a new one
  #endif


  // Delay between loop iterations
  #ifdef is_sleep_period
    loop_sleep();
  #endif
  
} // End loop section


