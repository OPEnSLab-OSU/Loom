
#ifndef LOOM_LORA_h
#define LOOM_LORA_h

#include "Loom_CommPlat.h"

#include <RH_RF95.h>
#include <RHReliableDatagram.h>

// LoRa Module pins

/// LoRa hip select pin
#define RFM95_CS  8
/// LoRa reset pin
#define RFM95_RST 4
/// LoRa interrupt pin
#define RFM95_INT 3  

/// LoRa radio frequence.
/// Hardware specific, Tx must match Rx
#define RF95_FREQ 915.0


// these will be defined in the config file
#define LORA_SELF_ADDRESS   01
#define LORA_FRIEND_ADDRESS 00


/// LoRa communication platform module
class Loom_LoRa : public LoomCommPlat
{

protected:

	///	Underlying rf95 object
	RH_RF95*			driver; 	
	/// Manager for driver		
	RHReliableDatagram*	manager;

	/// Device Address    (should this be part of LoomCommPlat? – maybe not as each platform handles addresses differently)
	uint8_t				address;
	/// Default address to send to
	uint8_t				friend_address;	

	/// Power level to send at
	uint8_t				power_level;
	/// Number of transmission retries allowed
	uint8_t				retry_count;
	/// Delay between transmission retries (in milliseconds)
	uint16_t			retry_timeout;

public:

	// --- CONSTRUCTOR ---
	Loom_LoRa( 	
			char*		module_name			=  "LoRa",  // LORA_Module_Name,  //*** String | comment
		
			uint16_t	max_message_len		=  RH_RF95_MAX_MESSAGE_LEN,  // LORA_Max_Message_Len,   //*** {RH_RF95_MAX_MESSAGE_LEN, .., .. }
			// CommScope subnet_scope 		=  SCOPE_SUBNET,  // LORA_Subnet_Scope,
			bool		compress_messages	=  true,  // LORA_Compress_Messages,

			uint8_t		address				=  LORA_SELF_ADDRESS,  // LORA_Address,
			uint8_t		friend_address		=  LORA_FRIEND_ADDRESS,  // LORA_Friend_Address,
			uint8_t		power_level 		=  23,  // LORA_Power_Level,   //*** 5-23 | (low to high power)
			uint8_t		retry_count			=  3,  // LORA_Retry_Count,
			uint16_t	retry_timeout		= 200  // LORA_Retry_Timeout 
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_LoRa();


	// --- PUBLIC METHODS ---

	void		print_config() override;

	// Build OSC Bundle from packet if any exists
	bool		receive_bundle(OSCBundle& bndl);


	// Send an OSC bundle
	// Should overload or use default args to optionally allow default address
	bool		send_bundle(OSCBundle& bndl, uint16_t destination);
	bool		send_bundle(OSCBundle& bndl);

	// Broadcast an OSC bundle
	void		broadcast_bundle(OSCBundle& bndl);


	void		set_address(uint addr);
	uint		get_address();

	void		set_friend_address(uint addr);
	uint		get_friend_address();

private:


};


#endif