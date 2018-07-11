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


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_pushingbox();
// bool setup_ethernet();,
// void sendToPushingBox(int num_fields, char *server_name, char *devid); // not sure what this is
void sendToPushingBox(OSCMessage &msg);
void sendToPushingBox(OSCBundle *bndl);

// ================================================================
// ===                          SETUP                           ===
// ================================================================


void setup_pushingbox() 
{
	// This should probably just be moved to Ethernet setup,
	// that way Ethernet, WiFi, and cellular all handle their own setup
	// -- Correction:
	// This calls setup_ethernet, and should be moved to Loom_begin()

	// #if is_ethernet == 1
	// 	LOOM_DEBUG_Println("Setting up ethernet");

	// 	if(!setup_ethernet()) {
	// 		LOOM_DEBUG_Println("Failed to setup ethernet");
	// 	}
	// #endif
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
	LOOM_DEBUG_Println("Sending to PushingBox");

	// should probably first build the string that is being sent, as that will be independent of platform
	// this will make the code shorter and cleaner


	// client object can probably just be a pointer to an ethernet or wifi client,
	// assume the code is otherwise the same (this way I don't have to deal will having 
	// an Ethernet of wifi client named the same thing (which doesn't work if you are trying to use both))


	#if is_ethernet == 1
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
				if ((i % 2) == 0)     client.print("&key" + String(i/2) + "=");
				else                  client.print("&val" + String(i/2) + "=");
				
				if (i-6 < msg.size()) client.print(get_data_value(&msg, i-6));
				else                  client.print("null");
			}

			client.println(" HTTP/1.1");

			// client.print("Host: "); client.println("api.pushingbox.com");
			client.println("Host: api.pushingbox.com"); 			// need to make sure this works

			client.println("User-Agent: Arduino");
			client.println();

			LOOM_DEBUG_Println("Data done sending");	 

			return;  // data sent successfully, no need to try another platform

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

	#endif // of #if is_ethernet == 1


	#if is_wifi == 1

		// Still need to implement this, might just make a function that takes
		// either an Ethernet or Wifi client to handle the two platforms,
		// as these two will probably be fairly similar.
		// Fona will have to have its own code though

	#endif // of #if is_wifi == 1 


	#if is_fona == 1
		
		LOOM_DEBUG_Println("Running PushingBox for Fona");

		// read website URL
		uint16_t statuscode;
		int16_t  length;
		char     url[512];

		// char url[255] = "http://api.pushingbox.com/pushingbox?devid=v7ECCEF7A460E57A"
		
		// client.print("&key0=sheetID");
		// 	client.print("&val0=" + String(spreadsheet_id));
		// 	client.print("&key1=tabID");
		// 	client.print("&val1=" + String(tab_id));
		// 	client.print("&key2=deviceID");
		// 	client.print("&val2=" + String(DEVICE) + String(INIT_INST));

		// http://api.pushingbox.com/pushingbox?devid=v7ECCEF7A460E57A&key0=sheetID&val0=1Hv2oME5sjumUXv36GtFV1Q7I83xnXu-f-ZrxUNsXS_U&key1=tabID&val1=Sheet12&key2=deviceID&val2=Unknown7&key3=FonaArg1&val3=fona_data&key4=FonaArg2&val4=more_fona_data&key5=null&val5=null&key6=null&val6=null&key7=null&val7=null&key8=null&val8=null&key9=null&val9=null&key10=null&val10=null&key11=null&val11=null&key12=null&val12=null&key13=null&val13=null&key14=null&val14=null&key15=null&val15=null



		sprintf(url, "http://api.pushingbox.com/pushingbox?devid=%s&key0=sheetID&val0=%s&key1=tabID&val1=%s&key2=deviceID&val2=%s%d&key3=FonaArg1&val3=%s&key4=FonaArg2&val4=%s", device_id, spreadsheet_id, tab_id, DEVICE, INIT_INST, "fona_data", "more_fona_data"); 
		for (int i = 5; i < 16; i++) {
			sprintf(url, "%s&key%d=null&val%d=null", url, i, i);
		}
		LOOM_DEBUG_Println("URL: ");
		LOOM_DEBUG_Println(url);

		flushSerial();
		// Serial.println(F("NOTE: in beta! Use small webpages to read!"));
		// Serial.println(F("URL to read (e.g. www.adafruit.com/testwifi/index.html):"));
		// Serial.print(F("http://")); readline(url, 254);
		// Serial.println(url);

		Serial.println(F("****"));
		if (!fona.HTTP_GET_start(url, &statuscode, (uint16_t *)&length)) {
			Serial.println("Failed!");
			return;
		}
		while (length > 0) {
			while (fona.available()) {
				char c = fona.read();

				// Serial.write is too slow, we'll write directly to Serial register!
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
		Serial.println(F("\n****"));
		fona.HTTP_GET_end();
		// break;
	


	#endif // of #if is_fona == 1

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






// // Sends key-value interleaved array of strings to PushingBox
// // Converts to bundle first
// void sendToPushingBox(String key_values [], int len)
// {
// 	OSCBundle tmpBndl;
// 	convert_key_value_array_to_bundle(key_values, &tmpBndl, "/packet/header", len, SINGLEMSG);
// 	sendToPushingBox(&tmpBndl);
// }

// // Sends key and value associated arrays of strings to PushingBox
// // Converts to bundle first
// template <typename T>
// void sendToPushingBox(String keys [], T values [], int len)
// {
// 	OSCBundle tmpBndl;
// 	convert_assoc_arrays_to_bundle(keys, values, &tmpBndl, "/packet/header", len, SINGLEMSG);
// 	sendToPushingBox(&tmpBndl);
// }










