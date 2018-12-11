// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================

void msg_router(OSCMessage &msg, int addrOffset);       


// ================================================================ 
// ===               CUSTOM FUNCTION PROTOTYPES                 === 
// ================================================================

// Add the prototypes of any custom functions that you want to be 
// callable by the message router when it finds a message with 
// the corresponding address



// ================================================================
// ===                     MESSAGE ROUTER                       ===
// ================================================================
//
// Used to route OSC messages to the correct function to handle it
// Preprocessor only includes routing for available functionality
//
// @param msg         OSC message to route subroutine that handles it
// @param addrOffset  used to determine where to start check of message header string
//
void msg_router(OSCMessage &msg, int addrOffset) 
{
	LOOM_DEBUG_Print("Message:  ");
	print_message(&msg, 0);

	routing_match = true;  // will be reset to false if the end of the function is reached


	#if LOOM_DEBUG == 1
		char buffer[100];
		msg.getAddress(buffer, addrOffset);
		LOOM_DEBUG_Println3("Will try to route on: '", buffer, "'"); 
	#endif

	#if is_multiplexer == 1
		if (msg.fullMatch("/GetSensors", addrOffset)) {
			msg.add(configuration.packet_header_string);
			LOOM_DEBUG_Println("Got a request for sensor list");
		}
		// Multiplexer builds list of plugged in sensors and replys to Max
		if (msg.dispatch("/GetSensors",   		send_sensor_list, 		addrOffset) ) return;
	#endif

	#if num_servos > 0  
		// Sets a specified servos to a specified position / degree
		if (msg.dispatch("/Servo/Set",    		handle_servo_msg, 		addrOffset) ) return;
	#endif
	#if num_steppers > 0
		// Moves a specified stepper motor to a specified number of steps in a particular direction
		if (msg.dispatch("/Stepper/Set",  		handle_stepper_msg, 	addrOffset) ) return;
	#endif
	#if is_relay == 1
		// Sets the state of the specified relay
		if (msg.dispatch("/Relay/State",  		handle_relay_msg, 		addrOffset) ) return;
	#endif
	#if is_mpu6050 == 1
		// Instructs an device to recalibrate its MPU6050
		if (msg.dispatch("/cal_MPU6050",  		calMPU6050_OSC, 		addrOffset) ) return;
	#endif
	#if is_neopixel == 1
		// Sets a specified Neopixel's color
		if (msg.dispatch("/Neopixel",     		set_color, 				addrOffset) ) return;
	#endif

	#if is_pushingbox == 1
		// Sends data to Google sheets via PushingBox
		if (msg.dispatch("/SendToPB", 			sendToPushingBox, 		addrOffset) ) return;
		// Sets which Google spreadsheet to upload to 
		if (msg.dispatch("/SetSpreadSheetID", 	set_spreadsheet_id, 	addrOffset) ) return;
		// Sets which tab of the Google spreadsheet to upload to 
		if (msg.dispatch("/SetTabID", 			set_tab_id,			 	addrOffset) ) return;
		// Set minimum delay between PushingBox uploads
		if (msg.dispatch("/SetPushMinDelay", 	set_push_min_delay, 	addrOffset) ) return;
	#endif
	
	#if is_wifi == 1
		// Credentials to connect to a WiFi network
		if (msg.dispatch("/wifiSetup/client", 	connect_to_new_wifi_network,	addrOffset) ) return;
		// Switch device to become WiFi access point
		if (msg.dispatch("/wifiSetup/AP",     	switch_to_AP, 			addrOffset) ) return;
		// If Max has requested the IP address of this device, response with IP
		if (msg.dispatch("/requestIP",        	broadcastIP,  			addrOffset) ) return;
		// Command instructing device to request settings from Max (requires Channel Manager)
		if (msg.dispatch("/getNewChannel",		get_new_channel,  		addrOffset) ) return;
		// Set whether or not to request settings from Max Channel Manager on startup
		if (msg.dispatch("/SetRequestSettings",	set_request_settings, 	addrOffset) ) return;
	#endif

	#if is_sd == 1
		// Set whether or not to log received bundles to SD card
		if (msg.dispatch("/EnableSD",	 	set_SD_logging, 		addrOffset) ) return;
	#endif
	

	#if enable_hub_scripts == 1
		// Creates a new script from a received message
		if (msg.dispatch("/newHubScript",		message_to_script, 		addrOffset) ) return;
		// Deletes the specified dynamic script if it exists
		if (msg.dispatch("/deleteHubScript",	delete_script, 			addrOffset) ) return;
		// Saves any current dynamic scripts to flash
		if (msg.dispatch("/saveDynamicScripts", save_dynamic_scripts, 	addrOffset) ) return;
	#endif



	// Save the configuration struct as it is currently
	if (msg.dispatch("/SaveConfig", 	save_config, 			addrOffset) ) return;

	// Received ping message, reply
	if (msg.dispatch("/Ping", 			ping_reply, 			addrOffset) ) return;

	// Change sample rate (i.e. is_sleep_period Watchdog sleep time)
	if (msg.dispatch("/SetSampleRate", 	set_sleep_period, 		addrOffset) ) return;


// THESE ARE MORE GENERAL COMMANDS, MOSTLY FROM SUBNET / GLOBAL 


	// If Max has polled for devices on the WiFi network 
	#if is_wifi
		if (msg.dispatch("/ChannelPoll", 		respond_to_poll_request, 	addrOffset) ) return;
	#endif


	// If a hub device has polled for devices on the network
	//     This might be merged with the above dispatch later
	#if advanced_interdev_comm == 1
		if (msg.dispatch("/DeviceChannelPoll", 	respond_to_device_poll, 	addrOffset) ) return;	// node receiving from hub 
		// Set the instance number of this device
		// if (msg.dispatch("/SetID", 					set_instance_num, 		addrOffset) ) return;
		// Set the channel of this device
		if (msg.dispatch("/SetChannel", 			set_channel, 			addrOffset) ) return;
	#endif

	#if (hub_node_type == 0) && (advanced_interdev_comm == 1)
		if (msg.dispatch("/DevicePollResponse", update_known_devices, 		addrOffset) ) return;	// hub receiving from node 
	#endif

			// Probably also do a msg.dispatch("PollResponse", populate_known_devices, addrOffset + #) 
			// where # is the number of characters taken up by the device name, number, and '/'' 


	#if LOOM_DEBUG == 1
		LOOM_DEBUG_Println3("No Message Routing Match Found for: '", buffer, "'");
	#endif

	routing_match = false;  // no match found

}

