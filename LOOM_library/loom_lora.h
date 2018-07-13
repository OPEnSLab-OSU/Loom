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
RHReliableDatagram manager(rf95, LORA_HUB_ADDRESS);


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_lora(RH_RF95 *rf95, RHReliableDatagram *manager);
void lora_receive_bundle(OSCBundle *bndl);
bool lora_send_bundle(OSCBundle *bndl);
bool lora_send_bundle_fragment(OSCBundle *bndl);

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
	#if hub_node_type == 0 // if hub
		pinMode(8, INPUT_PULLUP);
	#endif

	LOOM_DEBUG_Println("Initializing manager...");
	if (!manager->init()){
		LOOM_DEBUG_Println("init failed");
	}
	
	LOOM_DEBUG_Println("Setting Frequency...");
	if (!rf95->setFrequency(RF95_FREQ)) {
		LOOM_DEBUG_Println("setFrequency failed");
	}

	LOOM_DEBUG_Println("Setting up ethernet");
	#if hub_node_type == 0 // if hub
		if(!setup_ethernet()) {
			LOOM_DEBUG_Println("Failed to setup ethernet");
		}
	#endif
	
	LOOM_DEBUG_Println("Setting power...");
	rf95->setTxPower(23, false);
	LOOM_DEBUG_Println("LoRa setup finished!");
}




// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================



// --- LoRa Receive Bundle ---
//
// Fills an OSC bundle with messages converted from strings as
// received on LoRa
// Changes the state/contents of the bundle and of the String Array, data
//
// @param bndl  A pointer to the OSCBundle to be filled
//
// String data[NUM_FIELDS];

void lora_receive_bundle(OSCBundle *bndl)
{
	if (manager.available()) {
		uint8_t len = LORA_MESSAGE_SIZE;
		uint8_t from;
		uint8_t buf[LORA_MESSAGE_SIZE];
		memset(buf, '\0', LORA_MESSAGE_SIZE);
		if (manager.recvfromAck(buf, &len, &from)) {

			convert_OSC_string_to_bundle((char*)buf, bndl); 

			#if is_pushingbox == 1
				//If true, then the data being received is from the evaporimeter, which
				//is formatted differently as they use code not written by the CS Capstone team.
				if (((char)(buf[0])) == '/') {
					// convert_OSC_string_to_bundle((char*)buf, bndl); 
					for(int i = 0; i < MAX_FIELDS; i++)
						data[i] = get_data_value(bndl->getOSCMessage(0), i);
				} else {
					char str[LORA_MESSAGE_SIZE];
					String((char*)buf).toCharArray(str, sizeof(str)-1);
					char *token;
					char *savept = str;
					String cols[6] = {"IDtag", "RTC_time", "temp", "humidity", "loadCell", "vbat"};
					for(int i = 0; i < MAX_FIELDS; i+=2) {
						token = strtok_r(savept, ",", &savept);
						if(token != NULL) {
							data[i] = cols[i/2];
							data[i+1] = String(token);
						}
					} // of for
				} // of else 
			#endif
		} // of if (manager.recvfromAck(buf, &len, &from))
	} // of if (manager.available()) 
}






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
	convert_OSC_bundle_to_string(bndl, message);

	LOOM_DEBUG_Println(message);
	LOOM_DEBUG_Println2("Message length: ", message);
	LOOM_DEBUG_Println2("Max message length: ", RH_RF95_MAX_MESSAGE_LEN);
	LOOM_DEBUG_Print("Sending...");
	 
	if (manager.sendtoWait((uint8_t*)message, strlen(message), LORA_HUB_ADDRESS)) {
		LOOM_DEBUG_Println("Sent bundle through LoRa!");
		return true;
	} else {
		LOOM_DEBUG_Println("Failed to send bundle!");
		return false;
	}
}

bool lora_send_bundle_fragment(OSCBundle *bndl)
{
	LOOM_DEBUG_Println2("Bundle of size ", get_bundle_bytes(bndl));
	LOOM_DEBUG_Println(" Being split into smaller bundles");

	OSCBundle tmp_bndl;
	OSCMessage *tmp_msg;

	for (int i = 0; i < bndl->size(); i++) {
		tmp_msg = bndl->getOSCMessage(i);
		tmp_bndl.empty();
		tmp_bndl.add(*tmp_msg);
		lora_send_bundle(&tmp_bndl);
	}
}









