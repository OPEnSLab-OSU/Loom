
#ifndef LOOM_NRF_h
#define LOOM_NRF_h

#include "Loom_CommPlat.h"

#include <RF24Network.h>
#include <RF24.h>


// // these will be defined in the config file
#define NRF_SELF_ADDRESS   01
#define NRF_FRIEND_ADDRESS 00

#define NRF_MESSAGE_SIZE 120


class Loom_nRF : public LoomCommPlat
{

protected:
	
	/// Underlying nRF24L01(+) radio object 
	RF24*				radio;    
	/// Network layer manager for radio		
	RF24Network*		network;

	/// Device Address    (should this be part of LoomCommPlat? – maybe not as each platform handles addresses differently)
	uint8_t				address;
	/// Default address to send to
	uint8_t				friend_address;


	/// Data rate
	uint8_t				data_rate;
	/// Power level to send at
	uint8_t				power_level;
	/// Number of transmission retries allowed
	uint8_t				retry_count;
	/// Delay between transmission retries (in milliseconds)
	uint16_t			retry_timeout;

	/// Which level(s) of the network to broadcast to
	uint8_t				multicast_level;

public:

	// --- CONSTRUCTOR ---
	Loom_nRF( 	char*		module_name			= "nRF",
		
				uint16_t	max_message_len		= 120,//RH_RF95_MAX_MESSAGE_LEN,
				// CommScope subnet_scope 		= SCOPE_SUBNET,
				bool		compress_messages	= true,

				uint8_t		address 			= NRF_SELF_ADDRESS,
				uint8_t		friend_address 		= NRF_FRIEND_ADDRESS,

				uint8_t		data_rate			= 1, 	// 0: Default, 1: RF24_250KBPS, 2: RF24_1MBPS, 3: RF24_2MBPS 
				uint8_t		power_level			= 0,	// 0: Default, 1: RF24_PA_MIN, 2: RF24_PA_LOW, 3: RF24_PA_HIGH, 4: RF24_PA_MAX
				uint8_t		retry_count 		= 3,
				uint16_t	retry_timeout 		= 200,

				uint8_t		multicast_level		= 1
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

	void set_multicast_level(uint8_t level);
	uint8_t get_multicast_level();

private:


};


#endif // of #ifndef LOOM_NRF_h
