// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define MAX_FIELDS 32			// Maximum number of fields accepted by the PushingBox Scenario    


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
String data[MAX_FIELDS];

unsigned long lastPushMillis, currentPushMillis;  


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_pushingbox();
void sendToPushingBox(OSCMessage &msg);
void sendToPushingBox(OSCBundle *bndl);
void pushingbox_ethernet(char* args);
void pushingbox_wifi(char* args);
void pushingbox_fona(char* args);

// ================================================================
// ===                          SETUP                           ===
// ================================================================


void setup_pushingbox() 
{

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
	// Only send bundles if a minimum time (pushMillisDelay seconds) has passed since last upload
	#if pushMillisFilter == 1
		currentPushMillis = millis();
		if ( (currentPushMillis - lastPushMillis) < (1000*pushMillisDelay) ) {
			LOOM_DEBUG_Println("Hasn't been long enough since last PushingBox upload, skipping this one");	
			return; // has not been long enough yet, just return
		} else {
			lastPushMillis = currentPushMillis;
		}
	#endif // of pushMillisFilter

	if (msg.size() > 32) { // This also catches empty msgs, which seem to have a size around 1493 for some reason
		LOOM_DEBUG_Println("Message to large to send to PushingBox");
		return;
	}

	#if verify_family_match == 1
		char source[32];
		osc_extract_header_section(&msg, 1, source);
		if (strcmp(FAMILY STR(FAMILY_NUM), source) != 0) {
			LOOM_DEBUG_Println("Source device family did not match");
			return;
		}
	#endif


	LOOM_DEBUG_Println("Sending to PushingBox");


	// Get the device source from bundle header 
	char bundle_deviceID[20];
	osc_extract_header_section(&msg, 2, bundle_deviceID);


	// Build url arguments from bundle
	char args[1024];

	#if useHubTabID == 1
		// Use hub's stored tab ID (in config file) to define spreadsheet tab
		sprintf(args, "/pushingbox?devid=%s&key0=sheetID&val0=%s&key1=tabID&val1=%s&key2=deviceID&val2=%s", 
			device_id, spreadsheet_id, tab_id_complete, bundle_deviceID);
		// sprintf(args, "/pushingbox?devid=%s&key0=sheetID&val0=%s&key1=tabID&val1=%s%d&key2=deviceID&val2=%s%d", 
		// 	device_id, spreadsheet_id, tab_id_prefix, DEVICE, INIT_INST, ); 
	#else
		// Use bundle source to define spreadsheet tab suffix
		sprintf(args, "/pushingbox?devid=%s&key0=sheetID&val0=%s&key1=tabID&val1=%s%s&key2=deviceID&val2=%s", 
			device_id, spreadsheet_id, tab_id_prefix, bundle_deviceID, bundle_deviceID); 	
	#endif

	// Populate URL with the bundle kesy and values
	for (int i = 0, j = 3; (i < MAX_FIELDS-6) && (i < msg.size()); i+=2, j++) {
	    char buf1[30], buf2[30];
		(get_data_value(&msg, i  )).toCharArray(buf1, 30); 
		(get_data_value(&msg, i+1)).toCharArray(buf2, 30);
		sprintf(args, "%s&key%d=%s&val%d=%s", args, j, buf1, j, buf2);
	}
	LOOM_DEBUG_Println2("URL get args: ", args);


	// Send to PushingBox with enabled internet connection
	#if is_ethernet == 1
		pushingbox_ethernet(args);
		return;
	#endif

	#if is_wifi == 1
		pushingbox_wifi(args);
		return;
	#endif

	#if is_fona == 1
		pushingbox_fona(args);
		return;
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
	// #if LOOM_DEBUG == 1
	// 	print_bundle(&tmpBndl);
	// #endif
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
		}
		#if LOOM_DEBUG == 1 
		else {
			Serial.println("Failed to re-setup ethernet.");
		}
		#endif
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


