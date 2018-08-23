

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void Loom_begin();
void set_instance_num(OSCMessage &msg);
#if (is_wifi == 1) && (is_button)    // look into other behaviors if other platforms are enabled instead
	void check_button_held();
#endif
void loop_sleep();
void save_config(OSCMessage &msg);
void flash_led(int num, int time_high, int time_low);
void flash_led();
int milli_duration(int amount, TimeUnits units);

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

		#if dynamic_serial_output != 1
			while(!Serial);        // Ensure Serial is ready to go before anything happens in LOOM_DEBUG mode.
			delay(2000);
		#endif

		Serial.println("Initialized Serial!\n");
	#endif

	LOOM_DEBUG_Println3("Device: ", DEVICE, INIT_INST);
	LOOM_DEBUG_Println3("Family: ", FAMILY, FAMILY_NUM);

	#if wake_delay == 1
		// LOOM_DEBUG_Println("Delaying 5 seconds");
		delay(5000);
	#endif
	
	#if hub_node_type == 0
		LOOM_DEBUG_Println("Running as Hub");
	#else
		LOOM_DEBUG_Println("Running as Node");
	#endif

	LOOM_DEBUG_Println();

	// Set the button pin mode to input
	#ifdef is_button
		pinMode(button_pin, INPUT_PULLUP); 
	#endif
	
	// Setup sensors and actuators by calling the respective setups
	// #if num_analog > 0
	#if (num_analog > 0) || (is_m0 == 1)
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
		#if is_hx711 == 1
			setup_hx711();
		#endif
		#if is_lis3dh == 1
			setup_lis3dh();
		#endif
		#if is_mb1232 == 1
			setup_mb1232();
		#endif
		#if is_mpu6050 == 1
			// setup_mpu6050();   not sure why this is commented out
		#endif
		#if is_ms5803 == 1
			setup_ms5803();
		#endif
		#if is_sht31d == 1
			setup_sht31d();
		#endif
		#if is_tsl2591 == 1
			setup_tsl2591();
		#endif
		#if is_tsl2561 == 1
			setup_tsl2561();
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
// LOOM_DEBUG_Println("Flash currently disabled for testing!");

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

	// Data Logging Platform setups
	#if is_sd == 1
		setup_sd();
	#endif
	#if is_pushingbox == 1
		setup_pushingbox();
	#endif

	// If hub, poll for devices on network(s)
	#if (hub_node_type == 0) && (advanced_interdev_comm == 1)
		setup_network_info();
	#endif

	// Setup scripts if enabled
	#if enable_hub_scripts == 1
		setup_hub_scripts();
	#endif

	#if advanced_interdev_comm == 1
		LOOM_DEBUG_Println("Routing:");
		LOOM_DEBUG_Println2("  Global: ", STR(/) FAMILY);
		LOOM_DEBUG_Println2("  Subnet: ", STR(/) FAMILY STR(FAMILY_NUM));
		LOOM_DEBUG_Println2("  Device: ", configuration.packet_header_string);
	#endif

	#if is_wifi
		LOOM_DEBUG_Println("UDP Ports");
		LOOM_DEBUG_Println2("  Global: ", GLOBAL_PORT);
		LOOM_DEBUG_Println2("  Subnet: ", SUBNET_PORT);
		LOOM_DEBUG_Println2("  Device: ", configuration.config_wifi.devicePort);
	#endif



	// Flash the built-in LED indicating setup complete
	flash_led();

	#if fona_test == 1
		fona_test_loop();
	#endif
}


// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================


#if advanced_interdev_comm == 1
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
		// respond_to_poll_request(configuration.packet_header_string);
		OSCMessage tmp = new OSCMessage("tmp");
		respond_to_poll_request(tmp);
	#endif
}


// --- SET CHANNEL NUMBER ---
//
// Updates device's channel (instance number and port)
//
// @param msg  Received OSC message with new channel 
//
void set_channel(OSCMessage &msg) 
{
	int new_channel = msg.getInt(0);

	if ( (new_channel < 1) || (new_channel > 8) ) {
		LOOM_DEBUG_Println("Invalid new channel");
		return;
	}

	LOOM_DEBUG_Println4("Received command to change channel from ", (char)(configuration.instance_number + 64), " to ", (char)(new_channel + 64) );

	configuration.instance_number = new_channel;
	#if is_wifi == 1
		configuration.config_wifi.devicePort = configuration.config_wifi.subnetPort + new_channel;
	#endif

	LOOM_DEBUG_Println2("New Inst: ", configuration.instance_number);
	#if is_wifi == 1
		LOOM_DEBUG_Println2("New Port: ", configuration.config_wifi.devicePort);
		UdpDevice.stop();
		UdpDevice.begin(configuration.config_wifi.devicePort);
	#endif

	sprintf(configuration.packet_header_string, "%s%d\0", PacketHeaderString, configuration.instance_number);
	sprintf(global_packet_header_string,"%s",configuration.packet_header_string);
	
	LOOM_DEBUG_Println2("New address header: ", configuration.packet_header_string);


	// Reply to Max
	#if is_wifi == 1
		OSCMessage tmp = OSCMessage("tmp");
		respond_to_poll_request(tmp);
	#endif

}

#endif // of if advanced_interdev_comm == 1



// --- CHECK BUTTON HELD ---
//
// Checked each iteration of main loop if the device's button has been held
// If so, restart in access point mode
//
#if (is_button) && (is_wifi == 1)
void check_button_held()
{
	if ( (uint32_t)digitalRead(button_pin) ) {
		button_timer = 0;
	} else {
		#ifdef is_sleep_period
			button_timer += is_sleep_period;  // this should be done with millis
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
// @param msg  Just the header string routed from msg_router, not used here
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
void flash_led(int num, int time_high, int time_low)
{
	for (int i = 0; i < num; i++) {
		digitalWrite(led, HIGH);  
		delay(time_high);                       
		digitalWrite(led, LOW);   
		delay(time_low);  
	} 
	// digitalWrite(led, HIGH);
}

void flash_led() 
{
	flash_led(14, 40, 30);
}




// --- PING REPLY ---
// 
// Respond to ping
// 
void ping_reply(OSCMessage &msg)
{
	OSCBundle bndl;
	char address_string[80];
	sprintf(address_string, "/%s%d%s", FAMILY, FAMILY_NUM, "/PingResponse");


	#if is_wifi == 1
		wifi_send_bundle_subnet(&bndl);
	#endif

// Lora should have broadcast function in the future
	#if is_lora == 1
		bndl.add(address_string).add( (int32_t)lora_last_rssi );
		lora_send_bundle(&bndl);
	#endif

// Nrf should have broadcast function in the future
	#if is_nrf == 1
		nrf_send_bundle(&bndl);
	#endif

	bndl.empty();            
	LOOM_DEBUG_Println("Responded to ping");
}

	

// --- MILLI DURATION --- 
//
// Returns millisecond equivalent to provided 
// minutes, seconds, or milliseconds 
int second_duration(int amount, TimeUnits units) {
	switch (units) {
		case SECONDS: return amount; break;
		case MINUTES: return 60 * amount; break;
		default:      return 0;
	}
}



