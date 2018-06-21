// ================================================================
// ===                 COMMON GLOBAL VARIABLES                  ===
// ================================================================
int           led = LED_BUILTIN;              // LED pin number
volatile bool ledState = LOW;                 // State of LED
float         vbat = 3.3;                     // Place to save measured battery voltage (3.3V max)
char          packetBuffer[255];              // Buffer to hold incoming packet
char          ReplyBuffer[] = "acknowledged"; // A string to send back
OSCErrorCode  error;                          // Hold errors from OSC
uint32_t      button_timer;                   // For time button has been held
int 		  button_state;
char          addressString[255];


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void Loom_begin();
void set_instance_num(OSCMessage &msg);
//void msg_router(OSCMessage &msg, int addrOffset);       currently moved prototype to top of loom_preamble
#if (is_wifi == 1) && defined(button)
	void check_button_held();
#endif
void loop_sleep();
void save_config(OSCMessage &msg);
void flash_led();

// Main loop interface functions
void receive_bundle(OSCBundle *bndl, Platform platform);
void process_bundle(OSCBundle *bndl);
void measure_sensors();
void package_data(OSCBundle *send_bndl);
void send_bundle(OSCBundle *send_bndl, Platform platform, char* file);
void send_bundle(OSCBundle *send_bndl, Platform platform);

void additional_loop_checks();


// ================================================================
// ===                        LOOM BEGIN                        ===
// ================================================================
//
// Called by setup(), handles calling of any LOOM specific individual device setups
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

	#if is_tca9548a == 1
		setup_tca9548a();
	#endif
	#if is_decagon == 1
		deca_gs3_setup(); //rename this
	#endif
	#if is_mpu6050 > 0 && is_ishield == 1
		setup_mpu6050();
	#endif 
	#ifdef is_max31856
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

	// Read configuration from flash, or write config.h settings 
	// if no settings are currently saved
	flash_config_setup();

	// Communication Platform specific setups
	#if is_wifi == 1
		setup_wifi(configuration.packet_header_string);
	#endif
	#if is_lora == 1
		setup_lora(&rf95, &manager);
	#endif	
	#if is_nrf == 1
		setup_nrf();
	#endif
	#if is_sd == 1
		setup_sd();
	#endif
	#if is_pushingbox == 1
		setup_pushingbox();
	#endif

	// Flash the build in LED indicating setup complete
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
	
	#if LOOM_DEBUG == 1
		Serial.print("new address header: ");
		Serial.println(configuration.packet_header_string);
	#endif

	#if is_wifi == 1
		configuration.config_wifi.request_settings = 0; // Setting to 0 means that device will not request new port settings on restart. 
														// Note that configuration needs to be saved for this to take effect
		respond_to_poll_request(configuration.packet_header_string);
	#endif
}



// --- MESSAGE ROUTER ---
//
// Used to route OSC messages to the correct function to handle it
// Preprocessor only includes routing for available functionality
//
// @param msg         OSC message to route subroutine that handles it
// @param addrOffset  used to determine where to start check of message header string
//
void msg_router(OSCMessage &msg, int addrOffset) 
{
	#if LOOM_DEBUG == 1
		char buffer[100];
		msg.getAddress(buffer, addrOffset);
		Serial.print("Parsed ");
		Serial.println(buffer);
	#endif

//	#if is_sd == 1
//		if (SD_logging) {
//			sd_log_msg(&msg, "test_log");
//		}
//	#endif


	#if is_tca9548a
		if (msg.fullMatch("/GetSensors", addrOffset)) {
			msg.add(configuration.packet_header_string);
			#if LOOM_DEBUG == 1
				Serial.println("Got a request for sensor list");
			#endif
		}
		msg.dispatch("/GetSensors",   send_sensor_list, addrOffset);
	#endif
	#if num_servos > 0  
		msg.dispatch("/Servo/Set",    handle_servo_msg, addrOffset);
	#endif
	#if num_steppers > 0
		msg.dispatch("/Stepper/Set",  handle_stepper_msg, addrOffset);
	#endif
	#if is_relay == 1
		msg.dispatch("/Relay/State",  handle_relay_msg, addrOffset);
	#endif
	#if is_mpu6050 == 1 && is_ishield == 1
		msg.dispatch("/MPU6050/cal",  calMPU6050_OSC, addrOffset);
	#endif
	#if is_neopixel == 1
		msg.dispatch("/Neopixel",     set_color, addrOffset);
	#endif
	#if is_lora == 1 && lora_device_type == 0
		msg.dispatch("/SendToPB", sendToPushingBox, addrOffset);
	#endif
	
	#if is_wifi == 1
		msg.dispatch("/Connect/SSID",     	set_ssid,     			addrOffset);
		msg.dispatch("/Connect/Password", 	set_pass,				addrOffset);
		msg.dispatch("/wifiSetup/AP",     	switch_to_AP, 			addrOffset);
		msg.dispatch("/SetPort",          	set_port,     			addrOffset);
		msg.dispatch("/requestIP",        	broadcastIP,  			addrOffset);
		msg.dispatch("/getNewChannel",		new_channel,  			addrOffset);
		msg.dispatch("/SetRequestSettings",	set_request_settings, 	addrOffset);
	#endif

	#if is_sd == 1
		msg.dispatch("/enableSDlogging", enable_SD_logging, 		addrOffset);
	#endif
	
	msg.dispatch("/SetID", set_instance_num, addrOffset);
	msg.dispatch("/SaveConfig", save_config, addrOffset);
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
	} 
	else {
		#ifdef is_sleep_period
			button_timer += is_sleep_period;
		#else
			button_timer++;
		#endif
		if (button_timer >= 5000) { // ~about 8 seconds
			#if LOOM_DEBUG == 1
				Serial.println("Button held for 8 seconds, resetting to AP mode");
			#endif
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
	#else
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
	#if LOOM_DEBUG == 1
		Serial.println("Saving Configuration Settings");
		Serial.println("...");
	#endif

	write_non_volatile();

	#if LOOM_DEBUG == 1
		Serial.println("Done");
	#endif
}

// Flash the build in LED 
void flash_led()
{
	for (int i = 0; i < 8; i++) {
		digitalWrite(led, HIGH);  
		delay(80);                       
		digitalWrite(led, LOW);   
		delay(60);  
	} 
	digitalWrite(led, HIGH);
}



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
void receive_bundle(OSCBundle *bndl, Platform platform)
{
	bndl->empty();

	switch(platform) {
		#if is_wifi == 1
			case WIFI :
				// Handle wifi bundle if it exists
				// Checks device unique UDP port and common UDP port
				wifi_receive_bundle(bndl, &Udp,       configuration.config_wifi.localPort); 
				wifi_receive_bundle(bndl, &UdpCommon, configuration.config_wifi.commonPort);                             
															 
				// Compare the previous status to the current status
				wifi_check_status();
				break;
		#endif

		#if is_lora == 1 && lora_device_type == 0
			case LORA :
				lora_receive_bundle(bndl);
				break;
		#endif

		#if is_lora == 1 && lora_device_type == 2
			case LORA :
				//TODO: repeater functionality here
				break;
		#endif

		#if is_nrf == 1
			case NRF : 
				#if LOOM_DEBUG == 1
					Serial.println("Not yet implemented");
				#endif
				break;
		#endif

		// #if is_sd == 1
		// 	case SDCARD : 
		// 		Serial.println("Not yet implemented");
		// #endif
		#if LOOM_DEBUG == 1
		default :
				Serial.println("That platform is not enabled");
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
	
			#if LOOM_DEBUG == 1
				Serial.print("Number of items in bundle: ");
				Serial.println(bndl->size());
				Serial.print("First message address string: ");
				Serial.println(addressString);
			#endif

			// If SD logging is enabled and message was to this device, save bundle
			#if is_sd == 1 
				if ((SD_logging == 1) && (strncmp(addressString, packet_header_string, strlen(packet_header_string)) == 0)) {
					#if LOOM_DEBUG == 1
						Serial.println("Logging bundle");
					#endif
//					char fileName[50];
//					memset(fileName, '\0', sizeof(fileName));
//				 	sprintf(fileName, "%s%s", "data", "_log.txt");
//				 	replace_char(fileName, '/', '_');
//					String tmp = String(fileName);
//					memset(fileName, '\0', sizeof(fileName));
//					tmp.toCharArray(fileName, 30);
//					sd_save_bundle(fileName, bndl);
					sd_save_bundle("data_log.txt", bndl);

				}
			#endif

// void replace_char(char *str, char orig, char rep) 


			#if is_wifi == 1
				// Channel manager polls without device name so check is performed here
				// rather than in msg_router()
				if (strcmp(addressString, "/LOOM/ChannelPoll") == 0) {
					#if LOOM_DEBUG == 1
						Serial.println("Received channel poll request");
					#endif
					respond_to_poll_request(configuration.packet_header_string);
					return;
				}
			#endif
	
			#if is_wifi == 1
				//Clear the new_ssid and new_pass buffers
				for (int i = 0; i < 32; i++) {  
					state_wifi.new_ssid[i] = '\0';
					state_wifi.new_pass[i] = '\0';
				}
			#endif
	
	
			// Send the bndle to the routing function, which will route/dispatch messages to the currect handling functions
			// Most commands will be finished once control returns here (WiFi changes being handled below)
			bndl->route(configuration.packet_header_string, msg_router);

	
			#if is_wifi == 1
				// If new ssid and password have been received, try to connect to that network
				if (state_wifi.ssid_set == true && state_wifi.pass_set == true) {
					connect_to_new_network();   
				}
			#endif
	
		} else { // of !bndl.hasError()
			error = bndl->getError();
			#if LOOM_DEBUG == 1
				Serial.print("error: ");
				Serial.println(error);
			#endif
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
//	for (int i = 0 i < 

	
	// Get battery voltage
	vbat = analogRead(VBATPIN);
	vbat = (vbat * 2 * 3.3) / 1024; // We divided by 2, so multiply back, multiply by 3.3V, our reference voltage, div by 1024 to convert to voltage

	//	Get button state
	#ifdef button
		button_state = digitalRead(button);
	#endif
	
	//	Measure multiplexer sennsors
	#if is_tca9548a == 1
		measure_tca9548a();
		if (millis()-state_tca9548a.last_update_time > state_tca9548a.mux_update_period){
			update_sensors();
			#if LOOM_DEBUG == 1
				Serial.println("Update MUXShield Sensorlist");
			#endif
			state_tca9548a.last_update_time = millis();
		}
	#endif //is_tca9548a
	
	// Update MPU6050 Data
	#if is_ishield == 1 && is_mpu6050 == 1
		measure_mpu6050();      // Now measure MPU6050, update values in global registers 
	#endif //is_ishield && is_mpu6050

	// Update Thermocouple
	#if is_max31856 == 1
		measure_max31856();
	#endif

	// Get analog readings
	#if num_analog >= 1
		measure_analog();
	#endif

	// Get decagon readings
	#if is_decagon == 1
		measure_decagon();
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
	#if is_tca9548a == 1
		package_tca9548a(send_bndl,configuration.packet_header_string);
	#endif //is_tca9548a

	// Update MPU6050 Data
	#if is_ishield == 1 && is_mpu6050 == 1
		package_mpu6050(send_bndl,configuration.packet_header_string, 0);
	#endif //is_ishield && is_mpu6050

	#if is_max31856 == 1
		package_max31856(send_bndl,configuration.packet_header_string);
	#endif
	
	// Get analog readings
	#if num_analog >= 1
		package_analog(send_bndl,configuration.packet_header_string);
	#endif

	#if is_decagon == 1
		package_decagon(&send_bndl,configuration.packet_header_string);
	#endif
}





// --- SEND BUNDLE ---
//
// Sends a packaged bundle on the specified platform
// Is overloaded, 
// 
// @param send_bndl  The bundle to be sent
// @param platform   The wireless platform to send on, the values are
//                    encoded to Platform enum to reduce chance for errors
// @param file       The file name when saving to SD card
// 
void send_bundle(OSCBundle *send_bndl, Platform platform, char* file)
{
	switch(platform) {
		#if is_wifi == 1
			case WIFI :
				wifi_send_bundle(send_bndl);
				break;
		#endif

		#if is_lora == 1 && lora_device_type == 1
			case LORA :
				if (!lora_bundle_fragment) {
					
					lora_send_bundle(send_bndl);
					
				} else { // Separate bundle into smaller pieces
					#if LOOM_DEBUG == 1
						Serial.print("Bundle of size ");
						Serial.println(get_bundle_bytes(send_bndl));
						Serial.print(" Being split into smaller bundles");
					#endif
					
					OSCBundle tmp_bndl;
					OSCMessage *tmp_msg;
					
					for (int i = 0; i < send_bndl->size(); i++) {
						tmp_msg = send_bndl->getOSCMessage(i);
						tmp_bndl.empty();
						tmp_bndl.add(*tmp_msg);

						lora_send_bundle(&tmp_bndl);
					}
				}
				break;
		#endif

		#if is_nrf == 1
			case NRF : 
				#if LOOM_DEBUG == 1
					Serial.println("Not yet implemented");
				#endif
				break;
		#endif

		#if is_sd == 1
			case SDCARD : 
				Serial.println("saving bundle");
				sd_save_bundle(file, send_bndl);
				break;
		#endif
		
		#if LOOM_DEBUG == 1
		default :
				Serial.println("That platform is not enabled");
			#endif 
	} // of switch
}

void send_bundle(OSCBundle *send_bndl, Platform platform)
{
	send_bundle(send_bndl, platform, NULL);	
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

	// Delay between loop iterations
	#ifdef is_sleep_period
		loop_sleep();
	#endif
}


























