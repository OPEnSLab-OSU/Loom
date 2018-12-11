// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <RH_RF95.h>
#include <RHReliableDatagram.h>

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define LORA_MESSAGE_SIZE RH_RF95_MAX_MESSAGE_LEN       // Defaults to 251 as 4 bytes are used for the header.

// LoRa chip pins
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

RHReliableDatagram manager(rf95, LORA_SELF_ADDRESS);

int lora_last_rssi; // Only updated upon receiving a bundle



// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_lora(RH_RF95 *rf95, RHReliableDatagram *manager);
void lora_receive_bundle(OSCBundle *bndl);
bool lora_send_bundle(OSCBundle *bndl, uint16_t destination);
bool lora_send_bundle(OSCBundle *bndl);

// bool lora_send_bundle_fragment(OSCBundle *bndl);


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
	LOOM_DEBUG_Println("Setting up LoRa");

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


	// Set power level
	LOOM_DEBUG_Println("Setting power...");
	if ( (lora_power_level >= 5) && (lora_power_level <= 23) ) {
		rf95->setTxPower(lora_power_level, false);
		LOOM_DEBUG_Println2("Set LoRa power to ", lora_power_level);
	} else {
		rf95->setTxPower(23, false);
		LOOM_DEBUG_Println("Set LoRa power to 23");
	}

	// Set retry delay
	if (lora_retry_delay > 0) {
		manager->setTimeout(lora_retry_delay);
		LOOM_DEBUG_Println3("Set LoRa retry timeout to ", lora_retry_delay, " ms");
	} 

	// Set max retry amount
	if (lora_retry_count >= 0) {
		manager->setRetries(lora_retry_count);
		LOOM_DEBUG_Println2("Set LoRa max retry amount ", lora_retry_count);
	}

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

			// LOOM_DEBUG_Println2("Received: ", (const char*)buf);
			// LOOM_DEBUG_Println2("Len: ", len);

			// This is done just in case the compressed string
			// uncompresses to more than 251 characters
			char larger_buf[384];
			memset(larger_buf, '\0', sizeof(larger_buf));
			strcpy(larger_buf, (const char*)buf);

			convert_OSC_string_to_bundle((char*)larger_buf, bndl); 

			// Apply filtering based on family and subnet
			if ( !subnet_filter(bndl, lora_subnet_scope) ) {
				LOOM_DEBUG_Println("Received LoRa bundle out of scope");
			}

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
bool lora_send_bundle(OSCBundle *bndl, uint16_t destination)
{
	LOOM_DEBUG_Println2("Sending LoRa bundle to address: ", destination);

	char message[LORA_MESSAGE_SIZE];
	memset(message, '\0', sizeof(message));
	convert_OSC_bundle_to_string(bndl, message);

	// LOOM_DEBUG_Println(message);
	// LOOM_DEBUG_Println2("Message length: ", strlen(message));
	 
	bool is_sent = manager.sendtoWait((uint8_t*)message, strlen(message)+1, destination);

	LOOM_DEBUG_Println2("Send LoRa bundle " , (is_sent) ? "successful" : "failed" );

	lora_last_rssi = rf95.lastRssi(); 

	return is_sent;
}

bool lora_send_bundle(OSCBundle *bndl)
{
	lora_send_bundle(bndl, LORA_OTHER_ADDRESS); 
} 




// Compression added to reduce need for fragmenting


// bool lora_send_bundle_fragment(OSCBundle *bndl)
// {
// 	LOOM_DEBUG_Println2("Bundle of size ", get_bundle_bytes(bndl));
// 	LOOM_DEBUG_Println(" Being split into smaller bundles");

// 	OSCBundle tmp_bndl;
// 	OSCMessage *tmp_msg;

// 	for (int i = 0; i < bndl->size(); i+=5) {
// 		for (int j = 0; j < 5; j++) {
// 			if ((i+j) >= bndl->size()) break;
// 			tmp_msg = bndl->getOSCMessage(i+j);
// 			tmp_bndl.add(*tmp_msg);	
// 		}
// 		print_bundle(&tmp_bndl);
// 		lora_send_bundle(&tmp_bndl);
// 		tmp_bndl.empty();
// 	}
// }





