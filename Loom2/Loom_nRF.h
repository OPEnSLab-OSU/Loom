
#ifndef LOOM_NRF_h
#define LOOM_NRF_h

#include "Loom_CommPlat.h"

// #include <RH_RF95.h>
// #include <RHReliableDatagram.h>

// // LoRa chip pins
// #define RFM95_CS  8
// #define RFM95_RST 4
// #define RFM95_INT 3   // Use this for the M0 (7 for 32u4)

// #define RF95_FREQ 915.0	// Hardware specific, Tx must match Rx   Move to config


// // these will be defined in the config file
#define NRF_SELF_ADDRESS   01
#define NRF_FRIEND_ADDRESS 00



class Loom_nRF : public LoomCommPlat
{

protected:
	// RH_RF95 *            driver; 			//	rf95(RFM95_CS, RFM95_INT);
	// RHReliableDatagram * manager; 			//	manager(rf95, 00);

	uint8_t              address; 			// Device Address    (should this be part of LoomCommPlat? – maybe not as each platform handles addresses differently)
	uint8_t              friend_address;	// Default address to send to

	uint8_t	             power_level;
	uint8_t              retry_count;
	uint16_t             retry_timeout; 	// in milliseconds


public:

	// --- CONSTRUCTOR ---
	Loom_nRF( 	char*     module_name 		= "nRF",
		
				uint16_t  max_message_len 	= 120,//RH_RF95_MAX_MESSAGE_LEN,
				// CommScope subnet_scope 		= SCOPE_SUBNET,
				bool      compress_messages = true,

				uint8_t   address           = NRF_SELF_ADDRESS,
				uint8_t   friend_address    = NRF_FRIEND_ADDRESS,
				uint8_t   power_level 		= 23,
				uint8_t   retry_count       = 3,
				uint16_t  retry_timeout 	= 200
			);

	// --- DESTRUCTOR ---
	virtual ~Loom_nRF();


	void print_config();

	// Build OSC Bundle from packet if any exists
	bool receive_bundle(OSCBundle& bndl);


	// Send an OSC bundle
	// Should overload or use default args to optionally allow default address
	bool send_bundle(OSCBundle& bndl, uint16_t destination);
	bool send_bundle(OSCBundle& bndl);

	// Broadcast an OSC bundle
	void broadcast_bundle(OSCBundle& bndl);


	void set_address(uint addr);
	uint get_address();

	void set_friend_address(uint addr);
	uint get_friend_address();

private:


};


#endif // of #ifndef LOOM_NRF_h
