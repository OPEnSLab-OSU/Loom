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
String data[NUM_FIELDS];


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_pushingbox();
bool setup_ethernet();
void sendToPushingBox(int num_fields, char *server_name, char *devid);


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
	#if LOOM_DEBUG == 1
		Serial.println("Setting up ethernet");
	#endif
	if(!setup_ethernet()) {
		#if LOOM_DEBUG == 1
			Serial.println("Failed to setup ethernet");
		#endif
	}
}


// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================



// --- SEND TO PUSHINGBOX ---
// 
// Sends a get request to PushingBox
//
// @param msg  The message containing the information to send to PB.
//
void sendToPushingBox(OSCMessage &msg) 
{
	#if LOOM_DEBUG == 1
		Serial.println("Sending to pushing box");
	#endif
	client.stop();
	if (client.connect(server_name, 80)) {  
		client.print("GET /pushingbox?devid="); client.print(device_id); 
		for(int i = 0; i < NUM_FIELDS; i++) {
			if((i % 2) == 0)
				client.print("&key" + String(i/2) + "=");
			else
				client.print("&val" + String(i/2) + "=");
			client.print(get_data_value(&msg, i));
		}
		client.println(" HTTP/1.1");
		client.print("Host: "); client.println(server_name);
		client.println("User-Agent: Arduino");
		client.println();
	 
	} 
	else {
		#if LOOM_DEBUG == 1
			Serial.println("Failed to connect to PB, attempting to re-setup ethernet.");
		#endif
		if(setup_ethernet()) {
			#if LOOM_DEBUG == 1
				Serial.println("Successfully re-setup ethernet.");
			#endif
		}
		#if LOOM_DEBUG == 1 
			else {
				Serial.println("Failed to re-setup ethernet.");
			}
		#endif
	}
}


