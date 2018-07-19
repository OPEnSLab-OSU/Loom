// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================

void msg_router(OSCMessage &msg, int addrOffset);       
void common_msg_router(OSCMessage &msg, int addrOffset);       

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
	#if LOOM_DEBUG == 1
		char buffer[100];
		msg.getAddress(buffer, addrOffset);
		LOOM_DEBUG_Println2("Parsed ", buffer); 
	#endif

	#if is_multiplexer
		if (msg.fullMatch("/GetSensors", addrOffset)) {
			msg.add(configuration.packet_header_string);
			LOOM_DEBUG_Println("Got a request for sensor list");
		}
		// Multiplexer builds list of plugged in sensors and replys to Max
		msg.dispatch("/GetSensors",   		send_sensor_list, 		addrOffset);
	#endif

	#if num_servos > 0  
		// Sets a specified servos to a specified position / degree
		msg.dispatch("/Servo/Set",    		handle_servo_msg, 		addrOffset);
	#endif
	#if num_steppers > 0
		// Moves a specified stepper motor to a specified number of steps in a particular direction
		msg.dispatch("/Stepper/Set",  		handle_stepper_msg, 	addrOffset);
	#endif
	#if is_relay == 1
		// Sets the state of the specified relay
		msg.dispatch("/Relay/State",  		handle_relay_msg, 		addrOffset);
	#endif
	#if is_mpu6050 == 1 && is_ishield == 1
		// Instructs an device to recalibrate its MPU6050
		msg.dispatch("/MPU6050/cal",  		calMPU6050_OSC, 		addrOffset);
	#endif
	#if is_neopixel == 1
		// Sets a specified Neopixel's color
		msg.dispatch("/Neopixel",     		set_color, 				addrOffset);
	#endif
	#if is_pushingbox == 1 && hub == 1
		// Sends data to Google sheets via PushingBox
		msg.dispatch("/SendToPB", 			sendToPushingBox, 		addrOffset);
	#endif
	
	#if is_wifi == 1
		// Credentials to connect to a WiFi network
		msg.dispatch("/Connect/SSID",     	set_ssid,     			addrOffset);
		msg.dispatch("/Connect/Password", 	set_pass,				addrOffset);
		// Switch device to become WiFi access point
		msg.dispatch("/wifiSetup/AP",     	switch_to_AP, 			addrOffset);
		// Sets the unique port for device to look for WiFi packets on
		msg.dispatch("/SetPort",          	set_port,     			addrOffset);
		// If Max has requested the IP address of this device, response with IP
		msg.dispatch("/requestIP",        	broadcastIP,  			addrOffset);
		// Command instructing device to request settings from Max (requires Channel Manager)
		msg.dispatch("/getNewChannel",		new_channel,  			addrOffset);
		// Set whether or not to request settings from Max Channel Manager on startup
		msg.dispatch("/SetRequestSettings",	set_request_settings, 	addrOffset);
	#endif

	#if is_sd == 1
		// Set whether or not to log received bundles to SD card
		msg.dispatch("/setSDlogging",	 	set_SD_logging, 		addrOffset);
	#endif
	
	// Set the instance number of this device
	msg.dispatch("/SetID", 					set_instance_num, 		addrOffset);
	// Save the configuration struct as it is currently
	msg.dispatch("/SaveConfig", 			save_config, 			addrOffset);

}






// For messages sent as "/<FAMILY>...", i.e.: "/LOOM..."
// rather than providing a device identifier
void common_msg_router(OSCMessage &msg, int addrOffset) 
{
	#if LOOM_DEBUG == 1
		char buffer[100];
		msg.getAddress(buffer, addrOffset);
		LOOM_DEBUG_Println2("Parsed ", buffer); 
	#endif


	// Set the instance number of this device
	msg.dispatch("/ChannelPoll", 	respond_to_poll_request, 		addrOffset);


			// Probably also do a msg.dispatch("PollResponse", populate_known_devices, addrOffset + #) 
			// where # is the number of characters taken up by the device name, number, and '/'' 

}

