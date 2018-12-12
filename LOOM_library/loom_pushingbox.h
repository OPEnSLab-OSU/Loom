// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define MAX_PB_FIELDS 50	// Maximum number of fields accepted by the PushingBox Scenario  
							// Used to be 32 - not sure if there is a limit now  


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct config_pushingbox_t {
	char spreadsheet_id[50]; 		// Spreadsheet ID

	char tab_id[24];			// Tab prefix

	int  minimum_upload_delay;		// Minimum delya between uploads
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
#if pushUploadFilter == 1 
	unsigned long lastPushMillis;//, currentPushMillis;  
#endif

struct config_pushingbox_t * config_pushingbox;
void link_config_pushingbox(struct config_pushingbox_t *flash_setup_pushingbox){
	config_pushingbox = flash_setup_pushingbox;
}


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_pushingbox();
void sendToPushingBox(OSCMessage &msg);
void sendToPushingBox(OSCBundle *bndl);
void pushingbox_ethernet(char* args);
void pushingbox_wifi(char* args);
void pushingbox_fona(char* args);
void set_spreadsheet_id(OSCMessage &msg);
void set_tab_id(OSCMessage &msg);
void set_push_min_delay(OSCMessage &msg);
bool apply_pushingbox_filters(OSCMessage* msg);
void get_spreadsheet_id(OSCMessage* msg, char* ssid);
#if pushUploadFilter == 1 
bool check_push_upload_filter();
#endif
bool check_message_size(OSCMessage* msg);
bool is_ping_message(OSCMessage* msg);
bool check_family_match(OSCMessage* msg);


// ================================================================
// ===                          SETUP                           ===
// ================================================================
void setup_pushingbox() 
{
	LOOM_DEBUG_Println("Setting up PushingBox");

	strcpy( config_pushingbox->spreadsheet_id, init_spreadsheet_id );
	strcpy( config_pushingbox->tab_id, init_tab_id );
	config_pushingbox->minimum_upload_delay = pushUploadMinDelay;

	LOOM_DEBUG_Println("PushingBox setup complete");
}


// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================


// --- SEND TO PUSHINGBOX ---
// 
// Sends a get request to PushingBox
// Expects arguments of OSC message to be formated as:
// key, value, key, value...
// As the message router forwards messages not bundles, this is called by msg_router
// If you need to send a bundle, simply call the function (which is overloaded)
// with a correctly formatted OSCBundle
//
// The function tries platforms in the following order:
//   Ethernet, WiFi, cellular
// as enabled. The function returns immediately upon successful transmission
// else will try alternate platforms 
// (this might become an option later, that is, whether or not to try other enabled platforms or not upon failure)
//
//
// @param msg  The message containing the information to send to PB.
//
void sendToPushingBox(OSCMessage &msg) 
{
 	// If any of the filters are not passed, do not upload
	if (!apply_pushingbox_filters(&msg)) return;

	LOOM_DEBUG_Println("Sending to PushingBox");


	char bundle_deviceID[20], args[1024], ss_id[48];
	bool use_default = false;

	// Get the device source from bundle header 
	osc_extract_header_section(&msg, 2, bundle_deviceID);

	// Determine spreadsheet ID
	get_spreadsheet_id(&msg, ss_id);

	// Build info needed by PushingBox and Google Script to route data
	sprintf(args, "/pushingbox?devid=%s&key0=sheetID&val0=%s&key1=tabID&val1=%s%s%s&key2=deviceID&val2=%s&key3=full_data&val3=", 
		device_id, 								// PushingBox Device ID
		ss_id,   								// Spreadsheet ID
		(use_default) ? "default_" : "",		// Whether or not to specify that this is default spreadsheet (only applies when using sheet array) 
		config_pushingbox->tab_id, 				// Tab to write to
		(useHubTabID) ? "" : bundle_deviceID, 	// Optional suffix of tab if node, not hub defines complete tab ID
		bundle_deviceID); 						// The bundle source's device ID

	// Populate URL with the bundle keys and values from the OSC bundle/message
	for (int i = 0, j = 3; (i < MAX_PB_FIELDS-6) && (i < msg.size()); i+=2, j++) {
		sprintf(args, "%s%s~%s~", args, get_data_value(&msg, i).c_str(), get_data_value(&msg, i+1).c_str() );
	}
	args[strlen(args)-1] = '\0'; // Delete trailing '~'


	LOOM_DEBUG_Println2("URL get args: ", args);
	LOOM_DEBUG_Println2("Len: ", strlen(args));


	// Send to PushingBox with enabled internet connection
	#if is_ethernet == 1
		pushingbox_ethernet(args);	return;
	#endif

	#if is_wifi == 1
		pushingbox_wifi(args);		return;
	#endif

	#if is_fona == 1
		pushingbox_fona(args);		return;
	#endif
}



// --- SEND TO PUSHINGBOX ---
// 
// Sends a get request to PushingBox
// Expects OSC bundle to only have 1 message and its arguments to be formated as:
// key, value, key, value...
// Simple a wrapper for the version of this function that takes a message
// As the message router forwards messages not bundles
//
// @param msg  The message containing the information to send to PB.
//
void sendToPushingBox(OSCBundle *bndl) 
{
	OSCBundle tmpBndl;
	deep_copy_bundle(bndl, &tmpBndl);
	convert_bundle_structure(&tmpBndl, SINGLEMSG);
	sendToPushingBox( *(tmpBndl.getOSCMessage(0)) );
}




#if is_ethernet == 1
// --- PUSHINGBOX ETHERNET ---	
// 
// Handles the Ethernet specific sending to PushingBox
//
// @param args  The URL to be sent to PushingBox 
//
void pushingbox_ethernet(char* args)
{
	LOOM_DEBUG_Println("Running PushingBox for Ethernet");

	ethernet_client.stop();
	if (ethernet_client.connect("api.pushingbox.com", 80)) {  
		LOOM_DEBUG_Println("Connection good");

		ethernet_client.print("GET ");
		ethernet_client.print(args);
		ethernet_client.println(" HTTP/1.1\nHost: api.pushingbox.com\nUser-Agent: Arduino\n");

		LOOM_DEBUG_Println("Data done sending");

		return;  // data sent successfully, no need to try another platform

	} else {
		LOOM_DEBUG_Println("No Connection");
		LOOM_DEBUG_Println("Failed to connect to PB, attempting to re-setup ethernet.");

		if (setup_ethernet()) {
			LOOM_DEBUG_Println("Successfully re-setup ethernet.");
		} else {
			LOOM_DEBUG_Println("Failed to re-setup ethernet.");
		}
	}
}
#endif // of #if is_ethernet == 1



#if is_wifi == 1
// --- PUSHINGBOX WIFI ---	
// 
// Handles the wifi specific sending to PushingBox
//
// @param args  The URL to be sent to PushingBox 
//
void pushingbox_wifi(char* args) 
{
	LOOM_DEBUG_Println("Running PushingBox for WiFi");

	LOOM_DEBUG_Println2("ARGS: ", args);
	LOOM_DEBUG_Println2("LEN: ", strlen(args));

	wifi_client.stop();
	if (wifi_client.connect("api.pushingbox.com", 80)) {  
		LOOM_DEBUG_Println("Connection good");

		wifi_client.print("GET "); 
		wifi_client.print(args);
		wifi_client.println(" HTTP/1.1\nHost: api.pushingbox.com\nUser-Agent: Arduino\n");

		LOOM_DEBUG_Println("Data done sending");	 
		
		return;  // data sent successfully, no need to try another platform

	} else {
		LOOM_DEBUG_Println("No WiFi Connection");
	}
}
#endif // of #if is_wifi == 1 



#if is_fona == 1
// --- PUSHINGBOX FONA ---	
// 
// Handles the Fona specific sending to PushingBox
//
// @param args  The most of the URL to be sent to PushingBox 
// 					This function adds a prefix to it
//
void pushingbox_fona(char* args)
{
	LOOM_DEBUG_Println("Running PushingBox for Fona");

	uint16_t statuscode;
	int16_t  length;
	char     fona_url[1049];

	sprintf(fona_url, "http://api.pushingbox.com%s", args);
	LOOM_DEBUG_Println2("URL: ", fona_url);

	flushSerial();
	if (!fona.HTTP_GET_start(fona_url, &statuscode, (uint16_t *)&length)) {
		LOOM_DEBUG_Println("Fona PushingBox failed!");
		return;
	}

	// Prints Fona reading of response, which doesn't show anything particularly interesting
	while (length > 0) {
		while (fona.available()) {
			char c = fona.read();
			#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
			loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
			UDR0 = c;
			#else
			Serial.write(c);
			#endif
			length--;
			if (! length) break;
		}
	}

	fona.HTTP_GET_end();
}
#endif // of #if is_fona == 1







// --- SET SPREADSHEET ID --- 
//
// Sets the spreadsheet Id - i.e. what spreadsheet to upload to
//
// @param msg  OSC messages that had header ending in '/SetSpreadSheetID'
//				Contains the new spreadsheet ID
//
void set_spreadsheet_id(OSCMessage &msg)
{
	if (msg.isString(0)) {
		msg.getString(0, config_pushingbox->spreadsheet_id, sizeof(config_pushingbox->spreadsheet_id));	
		write_non_volatile();
		LOOM_DEBUG_Println3("Setting Spreadsheet ID to: \"", config_pushingbox->spreadsheet_id, "\"");
	} else {
		LOOM_DEBUG_Println("Command to set spreadsheet id was not in correct format");
	}
}



// --- SET TAB ID --- 
//
// Sets the Tab Id - i.e. what sheet to upload to
//
// @param msg  OSC messages that had header ending in '/SetTabID'
//				Contains the new tab ID
//
void set_tab_id(OSCMessage &msg)
{
	if (msg.isString(0)) {
		msg.getString(0, config_pushingbox->tab_id, sizeof(config_pushingbox->tab_id));	
		write_non_volatile();
		LOOM_DEBUG_Println3("Setting Tab ID to: \"", config_pushingbox->tab_id, "\"");
	} else {
		LOOM_DEBUG_Println("Command to set tab id was not in correct format");
	}
}



// --- SET PUSHINGBOX MIN DELAY --- 
//
// Sets the minimum dleay between PushingBox uploads
//
// @param msg  OSC messages that had header ending in '/SetPushMinDelay'
//				Contains the new minimum delay
//
void set_push_min_delay(OSCMessage &msg)
{
	if (msg.isInt(0)) {
		config_pushingbox->minimum_upload_delay = msg.getInt(0);
		write_non_volatile();
		LOOM_DEBUG_Println3("Setting PushingBox Minimum Upload Delay to: ", config_pushingbox->minimum_upload_delay, " seconds");
	} else {
		LOOM_DEBUG_Println("Command to set PushingBox minimum upload delay was not in correct format");
	}
}



// --- APPLY PUSHINGBOX FILTERS ---
//
// Apply any filters that might prevent uploading a message to PushingBox
//
// @param msg  The message to check filters against
//
// @return True if all filters are passed, False otherwise
//
bool apply_pushingbox_filters(OSCMessage* msg)
{
	// Only send bundles if a minimum time (pushUploadMinDelay seconds) has passed since last upload
	#if pushUploadFilter == 1
		if (!check_push_upload_filter()) return false;
	#endif // of pushUploadFilter

	// Filter out bundles that are too large
	if (!check_message_size(msg)) 		return false;

	// Don't upload any ping messages
	if (is_ping_message(msg)) 			return false;

	// If option enabled, make sure device is in the same family
	#if verify_family_match == 1
		if (!check_family_match(msg)) 	return false;
	#endif
 
 	// Passed all filters
 	return true;
}



// -- GET SPREADSHEET ID ---
//
// Determines correct spreadsheet ID to send to 
//
// @param msg    The OSC message to be sent
// @param ss_id  The char buffer to fill with the ss_id 
//
void get_spreadsheet_id(OSCMessage* msg, char* ss_id)
{
	#if use_pb_sheet_array == 1 // Using list of spreadsheet_ids
		int msg_family_num = osc_extract_family_number(msg);

		// If out of range of array
		if (  (msg_family_num < 0) || (msg_family_num >= sizeof(spreadsheet_list)/sizeof(spreadsheet_list[0]) )  ) {
			strcpy(ss_id, pb_default_sheet); 						// Use default
			LOOM_DEBUG_Println("Family number out of spreadsheet list range, using default");
			use_default = true;
		} else {
			strcpy(ss_id, spreadsheet_list[msg_family_num]);	// Use ID from list
			LOOM_DEBUG_Println2("Using spreadsheet_id at index: ", msg_family_num);
		}
	#else // Use dynamic single spreadsheet_id
		strcpy(ss_id, config_pushingbox->spreadsheet_id);
	#endif
}



// --- CHECK PUSH UPLOAD FILTER ---
//
// Only send bundles if a minimum time (pushUploadMinDelay seconds) has passed since last upload
// This function checks if the message should be filtered out
//
// @return True if enough time has passed and the message can be uploaded, false otherwise
//
#if pushUploadFilter == 1
bool check_push_upload_filter()
{
	if ( (millis() - lastPushMillis) < (1000*config_pushingbox->minimum_upload_delay) ) {
		return false; // has not been long enough yet, just return
	} else {
		lastPushMillis = millis();
		return true;
	}
}
#endif // of pushUploadFilter




// --- CHECK MESSAGE SIZE ---
//
// Make sure message is not too large to send
// Also catches empty msgs, which seem to have a size around 1493 for some reason
//
// @param msg  The message to check the size of 
//
// @return True if message is of sendable size
bool check_message_size(OSCMessage* msg)
{
	if (msg->size() > MAX_PB_FIELDS) { // This also catches empty msgs, which seem to have a size around 1493 for some reason
		LOOM_DEBUG_Println("Message too large to send to PushingBox");
		return false;
	} else {
		return true;
	}
}

// --- IS PING MESSAGE ---
// 
// Make sure ping messages don't accidentally get uploaded
//
// @param msg 	OSC message to check if is ping related
//
// @return True if message was ping related
//
bool is_ping_message(OSCMessage* msg)
{
	char check_ping[50];
	osc_extract_header_section(msg, 2, check_ping);
	return strstr(get_address_string(msg).c_str(), "Ping");
}


// --- CHECK FAMILY MATCH ---
//
// Make sure message source is of the same family as this device
//
// @param msg 	OSC message to check family of
//
// @return True if the message's family matches
//
bool check_family_match(OSCMessage* msg)
{
	char source[32];
	osc_extract_header_section(msg, 1, source);
	return (strcmp(FAMILY STR(FAMILY_NUM), source) == 0);
}





