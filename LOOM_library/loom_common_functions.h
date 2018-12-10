

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
// void set_instance_num(OSCMessage &msg);
#if (is_wifi == 1) && (is_button)    // look into other behaviors if other platforms are enabled instead
	void check_button_held();
#endif
void loop_sleep();
void save_config(OSCMessage &msg);
void flash_led(int num, int time_high, int time_low);
void flash_led();
int  milli_duration(int amount, TimeUnits units);


bool subnet_filter(OSCBundle* bndl, int scope);

// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================


#if advanced_interdev_comm == 1

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
		int sleepMS = Watchdog.sleep(configuration.sample_rate); // Sleep MCU for transmit period duration
	#else // don't actually sleep if Debug mode on
		delay(configuration.sample_rate);                        // Dont actually go to sleep if connected to serial
	#endif

	// #if LOOM_DEBUG == 0
	// 	int sleepMS = Watchdog.sleep(is_sleep_period); // Sleep MCU for transmit period duration
	// #else // don't actually sleep if Debug mode on
	// 	delay(is_sleep_period);                        // Dont actually go to sleep if connected to serial
	// #endif
}
#endif


// Change the sleep period / sample rate (milliseconds)
void set_sleep_period(OSCMessage &msg)
{	
	// Currently only care about changing sample 
	//  rate of workshop devices with sensors
	#if (is_ishield == 1) || (is_multiplexer == 1) 
		int rate = msg.getInt(0);
		if ((rate < 20) || (rate > 16000)) {
			return;
		}

		configuration.sample_rate = rate;

		LOOM_DEBUG_Println3("Set the sample rate to ", rate, " milliseconds");
	#endif
}


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
		digitalWrite(LED_BUILTIN, HIGH);  
		delay(time_high);                       
		digitalWrite(LED_BUILTIN, LOW);   
		delay(time_low);  
	} 
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
		// wifi_send_bundle_subnet(&bndl); config_wifi->subnetPort
		wifi_send_bundle(&bndl, configuration.config_wifi.subnetPort); 
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




// --- SUBNET FILTER ---
//
// Called after receiving a bundle on a communication platform
// Does nothing if bundle is in specified scope
// Scope:
//   0: Filter all bundles
//   1: Subnet (family name and number have to match this device)
//   2: Family (only family name has to match this device)
//   3: Global only (accept messages that are of family scope but don't have a family number) 
//   4: Any
// 
// @param bndl   The bundle to check if in scope
// @param scope  What scope to accept in 
//
// @return True if bundle was in scope, false otherwise
//
bool subnet_filter(OSCBundle* bndl, int scope)
{
	char comp_buf[32], addr_section[32];

	switch (scope) {
		// Empty bundle
		case 0: bndl->empty(); return false;
		// Subnet
		case 1: 
			sprintf(comp_buf, "%s%d", FAMILY, FAMILY_NUM); 
			break;
		// Family / Global only
		case 2: case 3:
			sprintf(comp_buf, "%s", FAMILY); 
			break;
		// Any
		default: return true; 
	}

	osc_extract_header_section(bndl->getOSCMessage(0), 1, addr_section);

	if (    ( (scope == 2) && (strncmp(comp_buf, addr_section, strlen(comp_buf)) != 0) ) 
		 || ( (scope != 2) && (strcmp(comp_buf, addr_section) != 0) )  ) 
	{
			bndl->empty();
			return false;
	}
		
	return true;


}









