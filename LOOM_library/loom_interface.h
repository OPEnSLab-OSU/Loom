// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================

// Main loop interface functions
void receive_bundle(OSCBundle *bndl, CommPlatform platform);
void process_bundle(OSCBundle *bndl);
void measure_sensors();
void package_data(OSCBundle *send_bndl);
void send_bundle(OSCBundle *send_bndl, CommPlatform platform);
void log_bundle(OSCBundle *send_bndl, LogPlatform platform, char* file); //filename for SD files
void log_bundle(OSCBundle *send_bndl, LogPlatform platform);
bool bundle_empty(OSCBundle *bndl);

void additional_loop_checks();


// ================================================================ 
// ===                   INTERFACE FUNCTIONS                    === 
// ================================================================



// --- SEND BUNDLE ---
//
// Fills an OSC bundle with packets received the specified platform
// if data exists and platform is enabled
// 
// @param bndl       The bundle to fill
// @param platform   The wireless platform to receive on, the values are
//                    encoded to a Platform enum to reduce chance for errors
// 
void receive_bundle(OSCBundle *bndl, CommPlatform platform)
{
	bndl->empty();

	switch(platform) {
		#if is_wifi == 1
		case WIFI :
			// Handle wifi bundle if it exists
			// Checks device unique UDP port and common UDP port
			wifi_receive_bundle(bndl, &Udp,       configuration.config_wifi.localPort); 
			wifi_receive_bundle(bndl, &UdpCommon, configuration.config_wifi.commonPort); 
			break;
		#endif

		#if is_lora == 1
		case LORA : lora_receive_bundle(bndl); break;
		#endif

		#if is_nrf == 1
		case NRF : nrf_receive_bundle(bndl); break;
		#endif

		#if LOOM_DEBUG == 1
		default : LOOM_DEBUG_Println3("That platform (", platform, ") is not enabled to receiving");
		#endif 
	} // of switch
}





// --- PROCESS BUNDLE --- 
// 
// Examine the provided OSC bundle (presumably filled via receive_bundle()
// If bundle is not empty,  has no errors, and is addressed to this device, then
// attempt to perform action bundle specifies
// 
// @param bndl  The OSC bundle to be processed 
//
void process_bundle(OSCBundle *bndl)
{
	if (bndl->size()){
		// If no bundle error
		if (!bndl->hasError()) {
			char addressString[255];
			bndl->getOSCMessage(0)->getAddress(addressString, 0);
	
			LOOM_DEBUG_Println2("Number of items in bundle: ", bndl->size());
			LOOM_DEBUG_Println2("First message address string: ", addressString);

			// --- Message Routing ---
	
			// This is the most important part of this function 
			// Send the bndle to the routing function,
			// which will route/dispatch messages to the currect handling functions
			// Most commands will be finished once control returns here 
			bndl->route(configuration.packet_header_string, msg_router);
			
			// Also route on messages that were broadcasted without device identifier, only the Family
			bndl->route(STR(/) FAMILY, common_msg_router);


			// --- Miscellaneous Processing ---

			// If SD logging is enabled and message was to this device, save bundle
			#if is_sd == 1 
				if ((SD_logging == 1) && (strncmp(addressString, configuration.packet_header_string, strlen(configuration.packet_header_string)) == 0)) {
					LOOM_DEBUG_Println("Logging bundle");
					sd_save_bundle("bndl_log.txt", bndl, 2, 2);
				}
			#endif

		} else { // of !bndl.hasError()
			error = bndl->getError();
			LOOM_DEBUG_Println2("Bundle Error: ", error);
		} // of else
	} // of if (bndl->size())

	bndl->empty();
}





// --- MEASURE SENSORS ---
//
// Update stored readings from sensors by calling measure 
// on each enabled sensor
//
void measure_sensors()
{
	// Get battery voltage
	// vbat = analogRead(VBATPIN);
	vbat = read_analog(VBATPIN);
	vbat = (vbat * 2 * 3.3) / 4096; // We divided by 2, so multiply back, multiply by 3.3V, our reference voltage, div by 1024 to convert to voltage

	//	Get button state
	#ifdef button
		button_state = digitalRead(button);
	#endif
	
	//	Measure multiplexer sennsors
	#if is_multiplexer == 1
		measure_tca9548a();
		if (millis()-state_tca9548a.last_update_time > state_tca9548a.mux_update_period){
			update_sensors();
			LOOM_DEBUG_Println("Update MuxShield Sensorlist");
			state_tca9548a.last_update_time = millis();
		}
	#endif
	
	// Update MPU6050 Data
	#if is_ishield == 1 && is_mpu6050 == 1
		measure_mpu6050();      // Now measure MPU6050, update values in global registers 
	#endif 

	// Update Thermocouple
	#if is_max31856 == 1
		measure_max31856();
	#endif

	// Get analog readings
	#if (num_analog >= 1) && (is_sapflow != 1)
		measure_analog();
	#endif

	// Get decagon readings
	#if is_decagon == 1
		measure_decagon();
	#endif

	// Update time
	#if is_rtc == 1
		measure_rtc();
	#endif


	// If using I2C sensor without multiplexer
	#if is_multiplexer == 0
		#if is_lis3dh == 1
			measure_lis3dh();
		#endif
	#endif


	 // Get analog readings
	#if is_sapflow == 1 && hub_node_type == 1
		measure_sapflow();
		measure_sht31d();
		heat();
	#endif
}





// --- PACKAGE DATA ---
// 
// Fill the provided OSC bundle with latest stored sensor readings
//
// @param send_bndl  The OSC bundle to be filled
//
void package_data(OSCBundle *send_bndl)
{
	// Clear any previous contents
	send_bndl->empty();

	// Add battery data 
	sprintf(addressString, "%s%s", configuration.packet_header_string, "/vbat");
	send_bndl->add(addressString).add(vbat); 
	
	// Add button state
	#ifdef button
		sprintf(addressString, "%s%s", configuration.packet_header_string, "/button");
		send_bndl->add(addressString).add((int32_t)button_state);
	#endif

	//	Add multiplexer sensor data
	#if is_multiplexer == 1
		package_tca9548a(send_bndl,configuration.packet_header_string);
	#endif //is_multiplexer

	// Update MPU6050 Data
	#if is_ishield == 1 && is_mpu6050 == 1
		package_mpu6050(send_bndl,configuration.packet_header_string, 0);
	#endif //is_ishield && is_mpu6050

	#if is_max31856 == 1
		package_max31856(send_bndl,configuration.packet_header_string);
	#endif
	
	// Get analog readings
	#if (num_analog >= 1) && (is_sapflow != 1)
		package_analog(send_bndl,configuration.packet_header_string);
	#endif

	#if is_decagon == 1
		package_decagon(&send_bndl,configuration.packet_header_string);
	#endif

	#if is_sapflow == 1 && hub_node_type == 1
		package_sapflow(send_bndl,configuration.packet_header_string);
		package_sht31d(send_bndl,configuration.packet_header_string);
	#endif



	// Should add the other I2C sensor package functions here 
	// for when they are called without the multiplexer

	#if is_multiplexer != 1
		#if is_lis3dh == 1
			package_lis3dh(send_bndl,configuration.packet_header_string);
		#endif
	#endif
}





// --- SEND BUNDLE ---
//
// Sends a packaged bundle on the specified platform
// 
// @param send_bndl  The bundle to be sent
// @param platform   The wireless platform to send on, the values are
//                    encoded to Platform enum to reduce chance for errors
// @param file       The file name when saving to SD card
// 
// void send_bundle(OSCBundle *send_bndl, Platform platform, char* file)
void send_bundle(OSCBundle *send_bndl, CommPlatform platform)
{
	switch(platform) {
		#if is_wifi == 1
		case WIFI : wifi_send_bundle(send_bndl); break;
		#endif

		#if is_lora == 1
		case LORA : (!lora_bundle_fragment) ? lora_send_bundle(send_bndl) : lora_send_bundle_fragment(send_bndl); break;
			// if (!lora_bundle_fragment) {
			// 	lora_send_bundle(send_bndl);					
			// } else { // Separate bundle into smaller pieces
			// 	lora_send_bundle_fragment(send_bndl);
			// }
			// break;
		#endif

		#if is_nrf == 1
		case NRF : (!nrf_bundle_fragment) ? nrf_send_bundle(send_bndl) : nrf_send_bundle_fragment(send_bndl); break;
			// if (!nrf_bundle_fragment) {
			// 	nrf_send_bundle(send_bndl);					
			// } else { // Separate bundle into smaller pieces
			// 	nrf_send_bundle_fragment(send_bndl);
			// }
			// break;
		#endif
		
		#if LOOM_DEBUG == 1
		default : Serial.println("That platform is not enabled for sending");
		#endif 
	} // of switch
}



// --- LOG BUNDLE ---
//
// Used to log a bundle to SD card, Google sheets,
// Adafruit IO, or the serial monitor
//
// @param log_bndl  The bndl to be logged
// @param platform  On which platform to log the bundle
// @param file      If logging to SD card, name of file to save to
//
void log_bundle(OSCBundle *log_bndl, LogPlatform platform, char* file)
{
	if ( (!file) && (platform == SDCARD) ) { 
		LOOM_DEBUG_Println("Logging to SD card requires filename");
		return;
	}

	switch(platform) {
		#if is_sd == 1
		case SDCARD : 
			LOOM_DEBUG_Println("Saving bundle");
			sd_save_bundle(file, log_bndl, 0, sd_save_time_format); 
			return;
		#endif

		#if is_pushingbox == 1
		case PUSHINGBOX : 
			LOOM_DEBUG_Println("Sending bundle data to PushingBox");
			sendToPushingBox(log_bndl);
			return;	
		#endif

		// #if is_adafruitio == 1
		// 	case ADAFRUITIO : 
		// 		LOOM_DEBUG_Println("Error: Adafruit IO doesn't take a filename"); 
		// 		return;	
		// #endif

		#if LOOM_DEBUG == 1
		case SERIAL_MON :
			print_bundle(log_bndl);
			return;
		#endif

		#if LOOM_DEBUG == 1
		default :
			Serial.println("That platform is not enabled for logging");
		#endif 
	} // of switch
	LOOM_DEBUG_Println("That platform is not enabled for exporting");
}


void log_bundle(OSCBundle *log_bndl, LogPlatform platform)
{
	log_bundle(log_bndl, platform, NULL);
}



// --- ADDITIONAL LOOP CHECKS ---
//
// Performs any miscellaneous Loom tasks that happen each loop iteration
// but are not handled by any of the other 5 interface functions
//
void additional_loop_checks()
{
	// Reset to AP mode if button held for ~5 seconds
	#if defined(button) && (is_wifi == 1)
		check_button_held();      
	#endif

	#if is_wifi == 1
		// If new ssid and password have been received, try to connect to that network
		check_connect_to_new_network();

		// Compare the previous status to the current status
		if (config_wifi->wifi_mode == AP_MODE) {
			wifi_check_status();
		}

		//Clear the new_ssid and new_pass buffers in case new wifi settings were received
		clear_new_wifi_setting_buffers();
	#endif

	// Delay between loop iterations
	#ifdef is_sleep_period
		loop_sleep();
	#endif


	#if is_adafruitio == 1
		adafruitio_subscribe();
		adafruitio_publish();
	#endif

	#if hub_node_type == 0
		check_device_refresh_interval();
	#endif
}




