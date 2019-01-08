
// ================================================================ 
// ===                   FUNCTION PROTOTYPE                     === 
// ================================================================
void Loom_begin();

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
	pinMode(LED_BUILTIN, OUTPUT);   // Set the LED pin mode

	//Initialize serial and wait for port to open:
	#if LOOM_DEBUG == 1
		Serial.begin(SERIAL_BAUD);

		#if dynamic_serial_output != 1
			while(!Serial);        // Ensure Serial is ready to go before anything happens in LOOM_DEBUG mode.
			delay(2000);
		#endif

		Serial.println("Initialized Serial!\n");
	#endif


	#if wake_delay == 1
		delay(5000);
	#endif
	

	LOOM_DEBUG_Println3("Device: ", DEVICE, INIT_INST);
	LOOM_DEBUG_Println3("Family: ", FAMILY, FAMILY_NUM);

	#if hub_node_type == 0
		LOOM_DEBUG_Println("Running as Hub\n");
	#else
		LOOM_DEBUG_Println("Running as Node\n");
	#endif


	// Print uploaded configuration
	#if (display_config == 1) && (LOOM_DEBUG == 1)
		print_config();
	#endif



	// Set the button pin mode to input
	#ifdef is_button
		pinMode(button_pin, INPUT_PULLUP); 
	#endif
	

	// Setup sensors and actuators by calling the respective setups
	#if (is_analog == 1) || (is_m0 == 1)
		setup_analog();
	#endif
	#if is_multiplexer == 1
		setup_tca9548a();
	#endif
	#if is_decagon == 1
		setup_decagon(); 
	#endif
	// #if is_mpu6050 == 1 
	// #if (is_mpu6050 == 1) && (is_multiplexer != 1)
		// setup_mpu6050();
	// #endif 
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

	#if is_ads1231 == 1
		setup_ads1231();
	#endif	

// Currently moved to after Ethernet setup
	// #if is_rtc == 1
	// 	setup_rtc();
	// #endif


	// I2C Sensor setup if no mulitplexer
	// call setups of enabled sensors individually
	#if is_multiplexer != 1
		#if is_as726X == 1 
			setup_as726X();
		#endif
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
			setup_mpu6050();   //not sure why this is commented out
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


	// Aggregate device setup
	#if is_sapflow == 1
		setup_sapflow();
	#endif


	// Read configuration from flash, or write config.h settings 
	//   if no settings are already saved
	setup_flash_config();


	// Communication Platform specific setups
	//   after flash setup as network info may be saved
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



	#if is_rtc == 1
		setup_rtc();
	#endif

	// Data Logging Platform setups
	#if is_sd == 1
		setup_sd();
	#endif
	#if is_pushingbox == 1
		setup_pushingbox();
	#endif


	// If hub and advanced interdev. comm., 
	// poll for devices on network(s)
	#if (hub_node_type == 0) && (advanced_interdev_comm == 1)
		setup_network_info();
	#endif


	// Setup scripts if enabled
	#if enable_hub_scripts == 1
		setup_hub_scripts();
	#endif


	#if advanced_interdev_comm == 1
		LOOM_DEBUG_Println("Routing:");
		if (configuration.config_wifi.wifi_mode != AP_MODE)	{
			LOOM_DEBUG_Println2("  Global: ", STR(/) FAMILY);
			LOOM_DEBUG_Println2("  Subnet: ", STR(/) FAMILY STR(FAMILY_NUM));
		}
		LOOM_DEBUG_Println2("  Device: ", configuration.packet_header_string);
	#endif


	#if is_wifi
		LOOM_DEBUG_Println("UDP Ports");
		if (configuration.config_wifi.wifi_mode != AP_MODE)	{
			LOOM_DEBUG_Println2("  Global: ", GLOBAL_PORT);
			LOOM_DEBUG_Println2("  Subnet: ", SUBNET_PORT);
		}
		LOOM_DEBUG_Println2("  Device: ", configuration.config_wifi.devicePort);
	#endif


	#if is_oled
		setup_oled();
	#endif


	LOOM_DEBUG_Println("\n= = = = = Setup Complete = = = = =\n");

	// Flash the built-in LED indicating setup complete
	flash_led();


	// This runs the Fona test loop 
	// Press 'Q' in the loop to continue on to normal operation
	#if fona_test == 1
		fona_test_loop();
	#endif
}
