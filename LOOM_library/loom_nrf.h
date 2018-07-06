// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <RF24Network.h>
#include <RF24.h>

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define NRF_MESSAGE_SIZE 120

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
RF24 radio(5,6);                 // nRF24L01(+) radio attached using Getting Started board 
RF24Network network(radio);      // Network uses that radio


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_nrf();
void nrf_receive_bundle(OSCBundle *);
bool nrf_send_bundle(OSCBundle *);
bool nrf_send_bundle_fragment(OSCBundle *bndl);

// ================================================================
// ===                          SETUP                           ===
// ================================================================

void setup_nrf() 
{
	pinMode(8, INPUT_PULLUP);
 
	SPI.begin();
	radio.begin();
	network.begin(90, NRF_HUB_ADDRESS);
	
	LOOM_DEBUG_Println("Setup nrf!");
}

// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================


// --- NRF RECEIVE BUNDLE ---
//
// Checks if any data has been received via nRF,
// is so, reads it in, converts it to a bundle and stores it in
// the bundle passed in
//
// @param bndl  The OSC bundle to be filled
//
void nrf_receive_bundle(OSCBundle *bndl) 
{
	network.update();                      // Check the network regularly
	while ( network.available() ) {        // Is there anything ready for us?
		RF24NetworkHeader header;          // If so, grab it and print it out
		char message[NRF_MESSAGE_SIZE];
		memset(message, '\0', NRF_MESSAGE_SIZE);
		network.read(header,&message,NRF_MESSAGE_SIZE-1);

		convert_OSC_string_to_bundle(message, bndl);
		LOOM_DEBUG_Println(message);
	}
}


// --- NRF SEND BUNDLE ---
//
// Takes an OSC bundle to send over nRF
// after conversion to the proper format (string)
//
// @param bndl  The OSC bundle to send (will be converted to string)
//
bool nrf_send_bundle(OSCBundle *bndl) 
{
	char message[NRF_MESSAGE_SIZE];
	memset(message, '\0', NRF_MESSAGE_SIZE);
	
	convert_OSC_bundle_to_string(bndl, message);
	
	RF24NetworkHeader header(NRF_HUB_ADDRESS);
	bool is_sent = network.write(header,message,strlen(message));
	#if LOOM_DEBUG == 1
		if (is_sent) 
			Serial.println("NRF Bundle Send Suceeded!");
		else 		
			Serial.println("NRF Bundle Send Failed!");
	#endif
	return is_sent;
}

bool nrf_send_bundle_fragment(OSCBundle *bndl)
{
	LOOM_DEBUG_Println2("Bundle of size ", get_bundle_bytes(bndl));
	LOOM_DEBUG_Println(" Being split into smaller bundles");

	OSCBundle tmp_bndl;
	OSCMessage *tmp_msg;

	for (int i = 0; i < bndl->size(); i++) {
		tmp_msg = bndl->getOSCMessage(i);
		tmp_bndl.empty();
		tmp_bndl.add(*tmp_msg);
		nrf_send_bundle(&tmp_bndl);
	}
}









