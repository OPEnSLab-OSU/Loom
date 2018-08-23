// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <RH_RF95.h>
#include <RHReliableDatagram.h>

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
#if hub_node_type == 0
	RHReliableDatagram manager(rf95, LORA_HUB_ADDRESS);
#else
	RHReliableDatagram manager(rf95, LORA_NODE_ADDRESS);
#endif

int lora_last_rssi;

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

	#if hub_node_type == 0 && is_ethernet == 1 // if hub
		LOOM_DEBUG_Println("Setting up ethernet");
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
void lora_receive_bundle(OSCBundle *bndl)
{
	if (manager.available()) {
		uint8_t len = LORA_MESSAGE_SIZE;
		uint8_t from;
		uint8_t buf[LORA_MESSAGE_SIZE];
		memset(buf, '\0', LORA_MESSAGE_SIZE);
		if (manager.recvfromAck(buf, &len, &from)) {

			lora_last_rssi = rf95.lastRssi();

			// LOOM_DEBUG_Println("Receiving");
			// This is done just in case the compressed string
			// uncompresses to more than 251 characters
			char larger_buf[384];
			memset(larger_buf, '\0', sizeof(larger_buf));
			strcpy(larger_buf, (const char*)buf);

			LOOM_DEBUG_Println2("Received: ", larger_buf);
			LOOM_DEBUG_Println2("Len: ", strlen((const char*)larger_buf));

			convert_OSC_string_to_bundle((char*)larger_buf, bndl); 

			// convert_OSC_string_to_bundle((char*)buf, bndl); 
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
	char message[LORA_MESSAGE_SIZE];
	memset(message, '\0', sizeof(message));
	convert_OSC_bundle_to_string(bndl, message);

	LOOM_DEBUG_Println(message);
	LOOM_DEBUG_Println2("Message length: ", strlen(message));
	LOOM_DEBUG_Println2("Max message length: ", LORA_MESSAGE_SIZE);
	// LOOM_DEBUG_Print("Sending...");
	 

// Sending to RH_BROADCAST_ADDRESS (255) might allow for broadcasting,
// these arent acknowledged though, so not sure if I'll have to change anything,
// such as not using 'sendtoWait' if there even is an alternative
//   probably use the 'sendto' function
//   might also just be able to temporarily set retries / timeout to 0 or something ? ...
// lora send bundle could then take an address as well, and overload so that if none is provided,
//   default to broadcasting with the other function
	// Serial.println();
	// for (int i = 0; i < strlen(message); i++) {
	// 	Serial.print(message[i], HEX);
	// 	Serial.print(" ");
	// 	if ((i != 0) && (i % 16 == 0))
	// 		Serial.println();
	// }
	// Serial.println();
	// RH_RF95::printBuffer("Contents: ", (const uint8_t*)message, strlen(message)+1);

	#if hub_node_type == 0
		LOOM_DEBUG_Println("Sending to node...");
		if (manager.sendtoWait((uint8_t*)message, strlen(message)+1, LORA_NODE_ADDRESS)) {  // this should be better generalized, as to be able to send to nodes
	#else
		LOOM_DEBUG_Println("Sending to server...");
		if (manager.sendtoWait((uint8_t*)message, strlen(message)+1, LORA_HUB_ADDRESS)) {  // this should be better generalized, as to be able to send to nodes
	#endif
		LOOM_DEBUG_Println("Sent bundle through LoRa!");
		return true;
	} else {
		LOOM_DEBUG_Println("Failed to send bundle!");
		return false;
	}

	// Use 'recvfromAckTimeout' from reliable client example for ack confirmation
}



bool lora_send_bundle_fragment(OSCBundle *bndl)
{
	LOOM_DEBUG_Println2("Bundle of size ", get_bundle_bytes(bndl));
	LOOM_DEBUG_Println(" Being split into smaller bundles");

	OSCBundle tmp_bndl;
	OSCMessage *tmp_msg;


	for (int i = 0; i < bndl->size(); i+=5) {
		for (int j = 0; j < 5; j++) {
			if ((i+j) >= bndl->size()) break;
			tmp_msg = bndl->getOSCMessage(i+j);
			tmp_bndl.add(*tmp_msg);	
		}
		print_bundle(&tmp_bndl);
		lora_send_bundle(&tmp_bndl);
		tmp_bndl.empty();
	}
}





