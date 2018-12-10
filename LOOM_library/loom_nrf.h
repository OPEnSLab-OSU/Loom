// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <RF24Network.h>
#include <RF24.h>

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define NRF_MESSAGE_SIZE 120
// Though most documentation says 32 byte max


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
bool nrf_send_bundle(OSCBundle *bndl, uint16_t destination);
bool nrf_send_bundle(OSCBundle *);
bool nrf_multicast_bundle(OSCBundle *bndl, uint8_t level);
bool nrf_multicast_bundle(OSCBundle *bndl);

// bool nrf_send_bundle_fragment(OSCBundle *bndl);

// ================================================================
// ===                          SETUP                           ===
// ================================================================

void setup_nrf() 
{
	LOOM_DEBUG_Println("Setting up nRF");

	// pinMode(8, INPUT_PULLUP);
 
	SPI.begin();
	radio.begin();


	// Set data rate
	LOOM_DEBUG_Print("Setting nRF data rate to: ");
	switch (nrf_data_rate) {
		case -1: 
			LOOM_DEBUG_Println("Default (1Mbps)");
			break; 
		case 1: radio.setDataRate(RF24_250KBPS);
			LOOM_DEBUG_Println("250 kbps");
			break;
		case 2: radio.setDataRate(RF24_1MBPS);
			LOOM_DEBUG_Println("1 Mbps");
			break;
		case 3: radio.setDataRate(RF24_2MBPS);
			LOOM_DEBUG_Println("2 Mbps");
			break;
	}
	// LOOM_DEBUG_Println2("nRF data rate : ", radio.getDataRate() );


	// Set power level
	LOOM_DEBUG_Print("Setting nRF power level to: ");
	switch (nrf_power_level) {
		case -1: 
			LOOM_DEBUG_Println("Default");
			break; 
		case 1: radio.setPALevel(RF24_PA_MIN);
			LOOM_DEBUG_Println("RF24_PA_MIN (-18dBm)");
			break;
		case 2: radio.setPALevel(RF24_PA_LOW);
			LOOM_DEBUG_Println("RF24_PA_LOW (-12dBm)");
			break;
		case 3: radio.setPALevel(RF24_PA_HIGH);
			LOOM_DEBUG_Println("RF24_PA_HIGH (-6dBM)");
			break;
		case 4: radio.setPALevel(RF24_PA_MAX);
			LOOM_DEBUG_Println("RF24_PA_MAX (0dBm)");
			break;
	}
	// LOOM_DEBUG_Println2("nRF power level : ", radio.getPALevel() );


	// Set retry behavior
	if (  ( (nrf_retry_delay >= 0) && (nrf_retry_delay <= 15) ) &&
		  ( (nrf_retry_count >= 0) && (nrf_retry_count <= 15) )    ) {

		radio.setRetries(nrf_retry_delay, nrf_retry_count);

		LOOM_DEBUG_Println3("nRF retry delay: ", 250*nrf_retry_delay, " microseconds");
		LOOM_DEBUG_Println2("nRF retry count: ", nrf_retry_count);
	} 


	network.begin(90, NRF_SELF_ADDRESS); // determine good way to convert change to octal address?

	LOOM_DEBUG_Println("nRF Setup Complete");
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
		char buf[NRF_MESSAGE_SIZE];
		memset(buf, '\0', NRF_MESSAGE_SIZE);
		network.read(header, &buf, NRF_MESSAGE_SIZE-1);

		// LOOM_DEBUG_Println2("Compressed buf :", buf);

		// This is done just in case the compressed string
		// uncompresses to more than 251 characters
		char larger_buf[180];
		memset(larger_buf, '\0', sizeof(larger_buf));
		strcpy(larger_buf, (const char*)buf);

		convert_OSC_string_to_bundle((char*)larger_buf, bndl); 
		
		// Apply filtering based on family and subnet
		if ( !subnet_filter(bndl, nrf_subnet_scope) ) {
			LOOM_DEBUG_Println("Received nRF bundle out of scope");
		}

	} // of while ( network.available() )
}




// --- NRF SEND BUNDLE ---
//
// Takes an OSC bundle to send over nRF
// after conversion to the proper format (string)
//
// @param bndl  The OSC bundle to send (will be converted to string)
//
bool nrf_send_bundle(OSCBundle *bndl, uint16_t destination) 
{
	char message[NRF_MESSAGE_SIZE];
	memset(message, '\0', NRF_MESSAGE_SIZE);
	
	convert_OSC_bundle_to_string(bndl, message);
	
	RF24NetworkHeader header(destination);									

	bool is_sent = network.write( header, message, strlen(message) );

	LOOM_DEBUG_Println2( "Send nRF bundle " , (is_sent) ? "successful" : "failed" );

	return is_sent;
}

bool nrf_send_bundle(OSCBundle *bndl)
{
	nrf_send_bundle(bndl, NRF_OTHER_ADDRESS);
} 



// Broadcasts a bundle

// Should it be merged with normal nrf send bundle?

bool nrf_multicast_bundle(OSCBundle *bndl, uint8_t level) 
{
	char message[NRF_MESSAGE_SIZE];
	memset(message, '\0', NRF_MESSAGE_SIZE);
	
	convert_OSC_bundle_to_string(bndl, message);
	
	RF24NetworkHeader header(00);									

	bool is_sent = network.multicast( header, message, strlen(message), level );

	LOOM_DEBUG_Println2("Multicast nRF bundle " , (is_sent) ? "successful" : "failed" );

	return is_sent;
}

bool nrf_multicast_bundle(OSCBundle *bndl)
{
	nrf_multicast_bundle(bndl, 1);
}





// Compression added to reduce need for fragmenting
// nRF library does fragmentation anyways (not sure if it is automatic)

// bool nrf_send_bundle_fragment(OSCBundle *bndl)
// {
// 	LOOM_DEBUG_Println2("Bundle of size ", get_bundle_bytes(bndl));
// 	LOOM_DEBUG_Println(" Being split into smaller bundles");

// 	OSCBundle  tmp_bndl;
// 	OSCMessage *tmp_msg;

// 	for (int i = 0; i < bndl->size(); i++) {
// 		tmp_msg = bndl->getOSCMessage(i);
// 		tmp_bndl.empty();
// 		tmp_bndl.add(*tmp_msg);
// 		nrf_send_bundle(&tmp_bndl);
// 	}
// }









