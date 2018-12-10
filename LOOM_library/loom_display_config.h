
// ================================================================ 
// ===                   FUNCTION PROTOTYPE                     === 
// ================================================================


// in Loom 2.0, each function should have a display_config() 
// method to reduce the number updates that need to be made here

void print_config()
{
	LOOM_DEBUG_Println("\n\n= = = = = Start Configuration Display = = = = =\n");


	LOOM_DEBUG_Println("Device Identification");
	LOOM_DEBUG_Println2("\tFamily: ", FAMILY);
	LOOM_DEBUG_Println2("\tFamily Number: ", FAMILY_NUM);
	LOOM_DEBUG_Println2("\tChannel: ", CHANNEL);
	LOOM_DEBUG_Println2("\tDevice: ", DEVICE);
	#if REQUEST_SETTINGS == 1
		LOOM_DEBUG_Println("\tWill Request Settings from Max");
	#endif



	LOOM_DEBUG_Println("Serial Options");
	#if dynamic_serial_output == 1
		LOOM_DEBUG_Println("\tDynamic Serial Output On");							
	#endif
	#if wake_delay == 1
		LOOM_DEBUG_Println("\tDelay upon waking enabled");							
	#endif							
	LOOM_DEBUG_Println2("\tSerial Baud: ", SERIAL_BAUD);


	LOOM_DEBUG_Println("Telemetry Options");
	#if hub_node_type == 0
		LOOM_DEBUG_Println("\tRunning as Hub");
	#else
		LOOM_DEBUG_Println("\tRunning as Node");
	#endif
	#if is_repeater == 1
		LOOM_DEBUG_Println("\tIs Repeater");							
	#endif	


	LOOM_DEBUG_Println("Aggregate Devices Defined");
	#if is_ishield == 1
		LOOM_DEBUG_Println("\tIs Ishield");							
	#endif	
	#if is_multiplexer == 1
		LOOM_DEBUG_Println("\tIs Multiplexer");							
	#endif	
	#if is_sapflow == 1
		LOOM_DEBUG_Println("\tIs Sapflow Meter");							
	#endif	
	#if is_evaporimeter == 1
		LOOM_DEBUG_Println("\tIs Evaporometer");							
	#endif	



	LOOM_DEBUG_Println("Communication Platforms");
	#if is_wifi == 1
		LOOM_DEBUG_Println("\tUses WiFi");							
	#endif	
	#if is_lora == 1
		LOOM_DEBUG_Println("\tUses LoRa");

		LOOM_DEBUG_Println2("\t\tSelf Address: ", LORA_SELF_ADDRESS);
		LOOM_DEBUG_Println2("\t\tDefault Other Address: ", LORA_OTHER_ADDRESS);

		// #if hub_node_type == 1
		// 	LOOM_DEBUG_Println2("\t\tNode Address: ", LORA_NODE_ADDRESS);
		// #endif						
		// LOOM_DEBUG_Println2("\t\tHub Address: ", LORA_HUB_ADDRESS);
	#endif	
	#if is_nrf == 1
		LOOM_DEBUG_Println("\tUses nRF");		

		LOOM_DEBUG_Println2("\t\tSelf Address: ", NRF_SELF_ADDRESS);
		LOOM_DEBUG_Println2("\t\tDefault Other Address: ", NRF_OTHER_ADDRESS);
					
	#endif	
	#if is_ethernet == 1
		LOOM_DEBUG_Println("\tUses Ethernet");	
		LOOM_DEBUG_Print("\t\tMAC: "); 
		LOOM_DEBUG_Print2(mac[0], ":");
		LOOM_DEBUG_Print2(mac[1], ":");
		LOOM_DEBUG_Print2(mac[2], ":");
		LOOM_DEBUG_Print2(mac[3], ":");
		LOOM_DEBUG_Print2(mac[4], ":");
		LOOM_DEBUG_Println(mac[5]);						
	#endif	
	#if is_fona == 1
		LOOM_DEBUG_Println("\tUses cellular");							
	#endif	
	#if is_bluetooth == 1
		LOOM_DEBUG_Println("\tUses Bluetooth");							
	#endif	



	LOOM_DEBUG_Println("Data Logging");
	#if is_pushingbox == 1
		LOOM_DEBUG_Println("\tPushingBox enabled");	
		LOOM_DEBUG_Println2("\t\tDevice ID: ", device_id);
		LOOM_DEBUG_Println2("\t\tSpreadsheet ID: ", init_spreadsheet_id);
		#if pushUploadFilter == 1	
			LOOM_DEBUG_Println2("\t\tMinimum Upload Delay (s): ", pushUploadMinDelay);	
		#endif
		// LOOM_DEBUG_Println2("\t\tSpreadsheet ID: ", config_pushingbox->spreadsheet_id);	
		// LOOM_DEBUG_Println2("\t\tMinimum Upload Delay (s): ", config_pushingbox->minimum_upload_delay);	
	#endif	
	#if is_adafruitio == 1
		LOOM_DEBUG_Println("\tAdafruit IO enabled");							
	#endif	
	#if is_sd == 1
		LOOM_DEBUG_Println("\tSD enabled");							
	#endif	
	#if is_rtc == 1
		LOOM_DEBUG_Print("\tRTC enabled");
		#if is_rtc3231 == 1
			LOOM_DEBUG_Println(" (via DS3231)");
		#elif is_rtc8523 == 1
			LOOM_DEBUG_Println(" (via PCF8523)");
		#endif							
	#endif	



	LOOM_DEBUG_Println("Actuators");
	#if num_servos > 0
		LOOM_DEBUG_Println3("\tServos enabled (", num_servos, ")");							
	#endif	
	#if num_steppers > 0
		LOOM_DEBUG_Println3("\tSteppers enabled (", num_steppers, ")");							
	#endif	
	#if is_relay == 1
		LOOM_DEBUG_Println3("\tRelay enabled (pin ", relay_pin, ")");							
	#endif	



	LOOM_DEBUG_Println("Sensors");
	#if is_analog == 1
		LOOM_DEBUG_Print("\tAnalog read enabled (pins ");
		#if is_analog_a0 == 1
			LOOM_DEBUG_Print("A0, ");
		#endif
		#if is_analog_a1 == 1
			LOOM_DEBUG_Print("A1, ");
		#endif
		#if is_analog_a2 == 1
			LOOM_DEBUG_Print("A2, ");
		#endif
		#if is_analog_a3 == 1
			LOOM_DEBUG_Print("A3, ");
		#endif
		#if is_analog_a4 == 1
			LOOM_DEBUG_Print("A4, ");
		#endif
		#if is_analog_a5 == 1
			LOOM_DEBUG_Print("A5, ");
		#endif
		LOOM_DEBUG_Println(")");
		LOOM_DEBUG_Println2("\t\tSamples to average: ", analog_samples);	
		LOOM_DEBUG_Println2("\t\tAnalog Resolution: ", analog_resolution);								
	#endif	

	#if is_button == 1
		LOOM_DEBUG_Println3("\tButton enabled (pin ", button_pin, ")");
	#endif

	#if is_decagon == 1
		LOOM_DEBUG_Print("\tDecagon enabled");
	#endif


	LOOM_DEBUG_Println("\tI2C Sensors:");



	#if is_tsl2591 == 1
		LOOM_DEBUG_Println("\t\t-TSL2591");
	#endif
	#if is_tsl2561 == 1
		LOOM_DEBUG_Println("\t\t-TSL2561");
	#endif
	#if is_fxos8700 == 1
		LOOM_DEBUG_Println("\t\t-FXOS8700");
	#endif
	#if is_fxas21002 == 1
		LOOM_DEBUG_Println("\t\t-FXAS21002");
	#endif
	#if is_zxgesturesensor == 1
		LOOM_DEBUG_Println("\t\t-ZXGestureSensor");
	#endif
	#if is_sht31d == 1
		LOOM_DEBUG_Println("\t\t-SHT31D");
	#endif
	#if is_mb1232 == 1
		LOOM_DEBUG_Println("\t\t-MB1232");
	#endif
	#if is_mpu6050 == 1
		LOOM_DEBUG_Println("\t\t-MPU6050");
	#endif
	#if is_lis3dh == 1
		LOOM_DEBUG_Println("\t\t-LIS3DH");
	#endif
	#if is_ms5803 == 1
		LOOM_DEBUG_Println("\t\t-MS5803");
	#endif
	#if is_hx711 == 1
		LOOM_DEBUG_Println3("\t\t-HX711 (calibration: ", hx711_calibration, ")");
	#endif
	#if is_as726X == 1
		LOOM_DEBUG_Println("\t\t-AS726X");
	#endif
	#if is_as7265X == 1
		LOOM_DEBUG_Println("\t\t-AS7265X");
	#endif

	#if is_lora == 0
		LOOM_DEBUG_Println2("Loop Delay / Sleep: ", is_sleep_period);
	#endif

	LOOM_DEBUG_Println("\n= = = = = End Configuration Display = = = = =\n");

}
