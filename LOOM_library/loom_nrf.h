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
RF24 radio(5,6);               	  // nRF24L01(+) radio attached using Getting Started board 
RF24Network network(radio);      	// Network uses that radio
// union data_value {
// 	int32_t i;
// 	float f;
// 	uint32_t u;
// };
// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_nrf();
void nrf_receive_bundle(OSCBundle *);
bool nrf_send_bundle(OSCBundle *);
void print_bundle(OSCBundle *bndl);

// ================================================================
// ===                          SETUP                           ===
// ================================================================

void setup_nrf() {
	pinMode(8, INPUT_PULLUP);
 
	SPI.begin();
	radio.begin();
	network.begin(90,NRF_HUB_ADDRESS);
	
	#if LOOM_DEBUG == 1
		Serial.println("Setup nrf!");
	#endif
}

// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================

void nrf_receive_bundle(OSCBundle *bndl) {
	network.update();                  // Check the network regularly
	while ( network.available() ) {     // Is there anything ready for us?
		RF24NetworkHeader header;        // If so, grab it and print it out
		char message[NRF_MESSAGE_SIZE];
		memset(message, '\0', NRF_MESSAGE_SIZE);
		network.read(header,&message,NRF_MESSAGE_SIZE-1);

		translate_string_to_OSC(message, bndl);
		#if LOOM_DEBUG ==1 
			Serial.println(message);
		#endif 
	}
}

bool nrf_send_bundle(OSCBundle *bndl) {
	char message[NRF_MESSAGE_SIZE];
	memset(message, '\0', NRF_MESSAGE_SIZE);
	translate_OSC_to_string(bndl, message);
	
	RF24NetworkHeader header(NRF_HUB_ADDRESS);
	bool is_sent = network.write(header,message,strlen(message));
	#if LOOM_DEBUG == 1
		if(is_sent) {
			Serial.println("NRF Bundle Send Suceeded!");
		}
		else {
			Serial.println("NRF Bundle Send Failed!");
		}
	#endif
	return is_sent;
}














