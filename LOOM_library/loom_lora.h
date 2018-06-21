// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
// #include <string.h>
#include <Ethernet2.h>

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define LORA_MESSAGE_SIZE RH_RF95_MAX_MESSAGE_LEN       // Defaults to 251 as 4 bytes are used for the header.
#define RFM95_CS 8
#define RFM95_RST 4

#ifdef is_m0
	#define RFM95_INT 3     // Use this for the M0
#endif
#ifdef is_32u4
	#define RFM95_INT 7     // Use this for the 32u4
#endif

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

RH_RF95 rf95(RFM95_CS, RFM95_INT);
RHReliableDatagram manager(rf95, SERVER_ADDRESS);


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_lora(RH_RF95 *rf95, RHReliableDatagram *manager);

#if lora_device_type == 0
	void lora_receive_bundle(OSCBundle *bndl);
#endif
#if lora_device_type == 1
	bool lora_send_bundle(OSCBundle *bndl);
#endif


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
void setup_lora(RH_RF95 *rf95, RHReliableDatagram *manager) 
{
	#if lora_device_type == 0
		pinMode(8, INPUT_PULLUP);
	#endif
	#if LOOM_DEBUG == 1
		Serial.println("Initializing manager...");
	#endif
	if (!manager->init()){
		#if LOOM_DEBUG == 1
			Serial.println("init failed");
		#endif
	}

	#if LOOM_DEBUG == 1
		Serial.println("Setting Frequency...");
	#endif
	if (!rf95->setFrequency(RF95_FREQ)) {
		#if LOOM_DEBUG == 1
			Serial.println("setFrequency failed");
		#endif
	}

	#if LOOM_DEBUG == 1
		Serial.println("Setting up ethernet");
	#endif
	#if lora_device_type == 0
		if(!setup_ethernet()) {
			#if LOOM_DEBUG == 1
				Serial.println("Failed to setup ethernet");
			#endif
		}
	#endif
	
	#if LOOM_DEBUG == 1
		Serial.println("Setting power...");
	#endif
	rf95->setTxPower(23, false);
	#if LOOM_DEBUG == 1
		Serial.println("LoRa setup finished!");
	#endif
}




// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================



#if lora_device_type == 0

// --- LoRa Receive Bundle ---
//
// Fills an OSC bundle with messages converted from strings as
// received on LoRa
// Changes the state/contents of the bundle and of the String Array, data
//
// @param bndl  A pointer to the OSCBundle to be filled
//
void lora_receive_bundle(OSCBundle *bndl)
{
	if (manager.available()) {
		uint8_t len = LORA_MESSAGE_SIZE;
		uint8_t from;
		uint8_t buf[LORA_MESSAGE_SIZE];
		memset(buf, '\0', LORA_MESSAGE_SIZE);
		if (manager.recvfromAck(buf, &len, &from)) {
			//If true, then the data being received is from the evaporimeter, which
			//is formatted differently as they use code not written by the CS Capstone team.
			if (((char)(buf[0])) == '/') {
				translate_string_to_OSC((char*)buf, bndl); 
				for(int i = 0; i < NUM_FIELDS; i++)
					data[i] = get_data_value(bndl->getOSCMessage(0), i);
			} else {
				char str[LORA_MESSAGE_SIZE];
				String((char*)buf).toCharArray(str, sizeof(str)-1);
				char *token;
				char *savept = str;
				String cols[6] = {"IDtag", "RTC_time", "temp", "humidity", "loadCell", "vbat"};
				for(int i = 0; i < NUM_FIELDS; i+=2) {
					token = strtok_r(savept, ",", &savept);
					if(token != NULL) {
						data[i] = cols[i/2];
						data[i+1] = String(token);
					}
				} // of for
			} // of else 
		} // of if (manager.recvfromAck(buf, &len, &from))
	} // of if (manager.available()) 
}

#endif // of lora_device_type == 0




#if lora_device_type == 1

// --- LORA SEND BUNDLE
//
// Sends an OSC Bundle over LoRa after converting the bundle 
// to an equivalent string
//
// @param bndl  OSC Bundle to send (will be converted to string)
//
// @return Whether or not bundle send was successful
//
bool lora_send_bundle(OSCBundle *bndl)
{
	char message[RH_RF95_MAX_MESSAGE_LEN];
	memset(message, '\0', sizeof(message));
	translate_OSC_to_string(bndl, message);
	#if LOOM_DEBUG == 1
		Serial.println(message);
		Serial.print("Message length: ");
		Serial.println(strlen(message));
		Serial.print("Max message length: ");
		Serial.println(RH_RF95_MAX_MESSAGE_LEN);
		Serial.print("Sending...");
	#endif
	 
	if (manager.sendtoWait((uint8_t*)message, strlen(message), SERVER_ADDRESS)) {
		#if LOOM_DEBUG == 1
			Serial.println("Sent bundle through LoRa!");
		#endif
		return true;
	}
	else {
		#if LOOM_DEBUG == 1
			Serial.println("Failed to send bundle!");
		#endif
		return false;
	}
}

#endif // of lora_device_type == 1



