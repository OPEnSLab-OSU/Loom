

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void Loom_begin();
void set_instance_num(OSCMessage &msg);
// void msg_router(OSCMessage &msg, int addrOffset);       
#if (is_wifi == 1) && defined(button)    // look into other behaviors if other platforms are enabled instead
	void check_button_held();
#endif
void loop_sleep();
void save_config(OSCMessage &msg);
void flash_led();



// ================================================================
// ===                        LOOM BEGIN                        ===
// ================================================================
//
// Called by setup(), handles calling of any LOOM specific individual device setups
// Runs on startup or upon device firmware upload
// Starts Wifi or Lora and serial if debugging prints are on
// Runs flash setup to read device settings from memory if available
// 
void Loom_begin()
{
	pinMode(led, OUTPUT);   // Set the LED pin mode

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
	
	// Setup sensors and actuators by calling the respective setups
	#if num_analog > 0
		setup_analog();
	#endif
	#if is_multiplexer == 1
		setup_tca9548a();
	#endif
	#if is_decagon == 1
		setup_decagon(); 
	#endif
	#if is_mpu6050 > 0 && is_ishield == 1
		setup_mpu6050();
	#endif 
	#if is_max31856 == 1
		setup_max31856();
	#endif 
	#if is_neopixel == 1
		setup_neopixel();
	#endif
	#if num_servos > 0
		setup_servo();
	#endif
	#if num_steppers > 0
		setup_stepper();
	#endif
	#if is_relay == 1
		setup_relay();
	#endif
	#if is_rtc == 1
		setup_rtc();
	#endif


	// I2C Sensor setup if no mulitplexer
	#if is_multiplexer != 1
		// call setups of enabled sensors
		#if is_fxas21002 == 1 
			setup_fxas21002();
		#endif
		#if is_fxos8700 == 1
			setup_fxos8700();
		#endif
		#if is_mb1232 == 1
			setup_mb1232();
		#endif
		#if is_mpu6050 == 1
			// setup_mpu6050();
		#endif
		#if is_sht31d == 1
			setup_sht31d();
		#endif
		#if is_tsl2591 == 1
			setup_tsl2591();
		#endif
		#if is_zxgesturesensor == 1
			setup_zxgesturesensor();
		#endif
	#endif


	// Prebuild device setup
	#if is_sapflow == 1
		setup_sapflow();
	#endif


	// Read configuration from flash, or write config.h settings 
	// if no settings are already saved
	setup_flash_config();


	// Communication Platform specific setups
	// after flash setup as network info may be saved
	#if is_wifi == 1
		setup_wifi(configuration.packet_header_string);
	#endif
	#if is_lora == 1
		setup_lora(&rf95, &manager);
	#endif	
	#if is_nrf == 1
		setup_nrf();
	#endif
	#if is_ethernet == 1
		setup_ethernet();
	#endif
	#if is_fona == 1
		setup_fona();
	#endif

	// Additional Platform setups
	#if is_sd == 1
		setup_sd();
	#endif
	#if is_pushingbox == 1
		setup_pushingbox();
	#endif

	// Flash the built-in LED indicating setup complete
	flash_led();
}


// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================



// --- SET INSTANCE NUMBER ---
//
// Updates device's identifying instance number.
// Device will not request new channel settings on startup if config is saved
// Device announces presence so running Channel Manager can track it
//
// @param msg  Received OSC message with new instance number
//
void set_instance_num(OSCMessage &msg) 
{
	configuration.instance_number = msg.getInt(0);
	sprintf(configuration.packet_header_string, "%s%d\0", PacketHeaderString, configuration.instance_number);
	sprintf(global_packet_header_string,"%s",configuration.packet_header_string);
	
	LOOM_DEBUG_Println2("New address header: ", configuration.packet_header_string);

	#if is_wifi == 1
		configuration.config_wifi.request_settings = 0; // Setting to 0 means that device will not request new port settings on restart. 
														// Note that configuration needs to be saved for this to take effect
		respond_to_poll_request(configuration.packet_header_string);
	#endif
}



// --- CHECK BUTTON HELD ---
//
// Checked each iteration of main loop if the device's button has been held
// If so, restart in access point mode
//
#if defined(button) && (is_wifi == 1)
void check_button_held()
{
	if ( (uint32_t)digitalRead(button) ) {
		button_timer = 0;
	} else {
		#ifdef is_sleep_period
			button_timer += is_sleep_period;
		#else
			button_timer++;
		#endif
		if (button_timer >= 5000) { // ~about 8 seconds
			LOOM_DEBUG_Println("Button held for 8 seconds, resetting to AP mode");
			button_timer = 0;
	 
			OSCMessage temp;          // Dummy message not used by function, but it expects an OSCMessage normally
			switch_to_AP(temp);       // Change to AP mode
		} 
	} // of else 
}
#endif 



// --- LOOM SLEEP ---
//
// Delay between iterations of the main loop
// Needed to ensure that bundles are fully sent and received
//
#ifdef is_sleep_period
void loop_sleep()
{
	#if LOOM_DEBUG == 0
		int sleepMS = Watchdog.sleep(is_sleep_period); // Sleep MCU for transmit period duration
	#else // don't actually sleep if Debug mode on
		delay(is_sleep_period);                        // Demo transmit every 1 second
	#endif
}
#endif



// --- SAVE CONFIG ---
//
// Saves the current configuration struct to non-volatile memory
//
// @param msg  Just header string routed from msg_router, not used here
//
void save_config(OSCMessage &msg) 
{
	LOOM_DEBUG_Println("Saving Configuration Settings");
	LOOM_DEBUG_Println("...");

	write_non_volatile();

	LOOM_DEBUG_Println("Done");
}



// --- FLASH LED ---
//
// Flash the build in LED 
// Helpful for indicating completion of events when debug is off
//
void flash_led()
{
	for (int i = 0; i < 14; i++) {
		digitalWrite(led, HIGH);  
		delay(40);                       
		digitalWrite(led, LOW);   
		delay(30);  
	} 
	digitalWrite(led, HIGH);
}





