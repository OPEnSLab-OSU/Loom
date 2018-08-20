// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================

void msg_router(OSCMessage &msg, int addrOffset);       
// void common_msg_router(OSCMessage &msg, int addrOffset);       

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

	// bool found = false;


	#if LOOM_DEBUG == 1
		char buffer[100];
		msg.getAddress(buffer, addrOffset);
		LOOM_DEBUG_Println3("Will try to route on: '", buffer, "'"); 
	#endif

	#if is_multiplexer
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
	#if is_mpu6050 == 1 && is_ishield == 1
		// Instructs an device to recalibrate its MPU6050
		if (msg.dispatch("/MPU6050/cal",  		calMPU6050_OSC, 		addrOffset) ) return;
	#endif
	#if is_neopixel == 1
		// Sets a specified Neopixel's color
		if (msg.dispatch("/Neopixel",     		set_color, 				addrOffset) ) return;
	#endif
	#if is_pushingbox == 1 && hub_node_type == 0
		// Sends data to Google sheets via PushingBox
		if (msg.dispatch("/SendToPB", 			sendToPushingBox, 		addrOffset) ) return;
	#endif
	
	#if is_wifi == 1
		// Credentials to connect to a WiFi network
		if (msg.dispatch("/Connect/SSID",     	set_ssid,     			addrOffset) ) return;
		if (msg.dispatch("/Connect/Password", 	set_pass,				addrOffset) ) return;
		// Switch device to become WiFi access point
		if (msg.dispatch("/wifiSetup/AP",     	switch_to_AP, 			addrOffset) ) return;
		// Sets the unique port for device to look for WiFi packets on
		if (msg.dispatch("/SetPort",          	set_port,     			addrOffset) ) return;
		// If Max has requested the IP address of this device, response with IP
		if (msg.dispatch("/requestIP",        	broadcastIP,  			addrOffset) ) return;
		// Command instructing device to request settings from Max (requires Channel Manager)
		if (msg.dispatch("/getNewChannel",		new_channel,  			addrOffset) ) return;
		// Set whether or not to request settings from Max Channel Manager on startup
		if (msg.dispatch("/SetRequestSettings",	set_request_settings, 	addrOffset) ) return;
	#endif

	#if is_sd == 1
		// Set whether or not to log received bundles to SD card
		if (msg.dispatch("/setSDlogging",	 	set_SD_logging, 		addrOffset) ) return;
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






// THESE ARE MORE GNERAL COMMANDS, MOSTLY FROM SUBNET / GLOBAL 


	// If Max has polled for devices on the WiFi network 
	#if is_wifi
		if (msg.dispatch("/ChannelPoll", 		respond_to_poll_request, 	addrOffset) ) return;
	#endif


	// If a hub device has polled for devices on the network
	//     This might be merged with the above dispatch later
	#if advanced_interdev_comm == 1
		if (msg.dispatch("/DeviceChannelPoll", 	respond_to_device_poll, 	addrOffset) ) return;	// node receiving from hub 
		// Set the instance number of this device
		if (msg.dispatch("/SetID", 					set_instance_num, 		addrOffset) ) return;
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
}






// // For messages sent as "/<FAMILY>...", i.e.: "/LOOM..."
// // rather than providing a device identifier
// void common_msg_router(OSCMessage &msg, int addrOffset) 
// {
// 	#if LOOM_DEBUG == 1
// 		char buffer[100];
// 		msg.getAddress(buffer, addrOffset);
// 		LOOM_DEBUG_Println2("Parsed ", buffer); 
// 	#endif

// 	// If Max has polled for devices on the WiFi network 
// 	#if is_wifi
// 		msg.dispatch("/ChannelPoll", 		respond_to_poll_request, 	addrOffset);
// 	#endif




// 	// If a hub device has polled for devices on the network
// 	//     This might be merged with the above dispatch later
// 	msg.dispatch("/DeviceChannelPoll", 	respond_to_device_poll, 	addrOffset);	// node receiving from hub

// 	#if hub_node_type == 0
// 		msg.dispatch("/DevicePollResponse", update_known_devices, 		addrOffset);	// hub receiving from node
// 	#endif

// 			// Probably also do a msg.dispatch("PollResponse", populate_known_devices, addrOffset + #) 
// 			// where # is the number of characters taken up by the device name, number, and '/'' 

// }

