// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
String data[MAX_FIELDS];


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_pushingbox();
// bool setup_ethernet();,
// void sendToPushingBox(int num_fields, char *server_name, char *devid);
void sendToPushingBox(OSCMessage &msg);
void sendToPushingBox(OSCBundle *bndl);

// ================================================================
// ===                          SETUP                           ===
// ================================================================

// ---  SETUP LORA ---
//
// Changes the state of the device to allow it to use LoRa. 
//
// @param rf95     An instance of the radio to be initialized
// @param manager  An instance of the manager to be initialized
//
void setup_pushingbox() 
{
	LOOM_DEBUG_Println("Setting up ethernet");
	#if is_ethernet == 1
		if(!setup_ethernet()) {
			LOOM_DEBUG_Println("Failed to setup ethernet");
		}
	#endif
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
//
// @param msg  The message containing the information to send to PB.
//
void sendToPushingBox(OSCMessage &msg) 
{
	LOOM_DEBUG_Println("Sending to PushingBox");

	client.stop();
	if (client.connect("api.pushingbox.com", 80)) {  
		LOOM_DEBUG_Println("Connection good");
		client.print("GET /pushingbox?devid="); client.print(device_id); 

		// Send information that should always be sent
		client.print("&key0=sheetID");
		client.print("&val0=" + String(spreadsheet_id));
		client.print("&key1=tabID");
		client.print("&val1=" + String(tab_id));
		client.print("&key2=deviceID");
		client.print("&val2=" + String(DEVICE) + String(INIT_INST));  // this will currently break if device number is altered via Max

		int i = 6; // offset by 6 to account for sheetId, tabID, deviceID

		for (; i < MAX_FIELDS; i++) {
			if ((i % 2) == 0) client.print("&key" + String(i/2) + "=");
			else              client.print("&val" + String(i/2) + "=");
			
			if (i-6 < msg.size()) client.print(get_data_value(&msg, i-6));
			else client.print("null");
		}

		client.println(" HTTP/1.1");
		client.print("Host: "); client.println("api.pushingbox.com");
		client.println("User-Agent: Arduino");
		client.println();

		LOOM_DEBUG_Println("Data done sending");	 
	} else {

		LOOM_DEBUG_Println("No Connection");
		#if is_ethernet == 1
			LOOM_DEBUG_Println("Failed to connect to PB, attempting to re-setup ethernet.");

			if (setup_ethernet()) {
				LOOM_DEBUG_Println("Successfully re-setup ethernet.");
			}
			#if LOOM_DEBUG == 1 
			else {
				Serial.println("Failed to re-setup ethernet.");
			}
			#endif
		#endif
	}
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
	LOOM_DEBUG_Println("Bundle going to PushingBox");
	// print_bundle(bndl);
	sendToPushingBox(*(bndl->getOSCMessage(0)));
}





