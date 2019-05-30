
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

// ### (LoomCommPlat) | dependencies: [] | conflicts: []
/// LoRa communication platform module
// ###
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

	/// LoRa module constructor
	///
	/// \param[in]	module_name					String | <"LoRa"> | LoRa module name
	/// \param[in]	max_message_len				Set(Int) | <255> | {255("Max length")} | The maximum possible message length
	/// \param[in]	address						Int | <01> | [0-99] | This device's LoRa address
	/// \param[in]	friend_address				Int | <00> | [0-99] | Default LoRa address to send to
	/// \param[in]	power_level					Int | <23> | [5-23] | Transmission power level, low to high power
	/// \param[in]	retry_count					Int | <3> | [0-15] | Max number of transmission retries
	/// \param[in]	retry_timeout				Int | <200>| [20-500] | Delay between retransmissions (ms)
	Loom_LoRa(
			const char*		module_name			= "LoRa",
			uint16_t		max_message_len		= RH_RF95_MAX_MESSAGE_LEN,
			uint8_t			address				= LORA_SELF_ADDRESS,
			uint8_t			friend_address		= LORA_FRIEND_ADDRESS,
			uint8_t			power_level 		= 23,
			uint8_t			retry_count			= 3,
			uint16_t		retry_timeout		= 200
		);

	Loom_LoRa(JsonVariant p);

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


	// Build json from packet if any exists
	bool		receive_json(JsonObject json);

	// Send json
	bool		send_json(JsonObject json, uint16_t destination);
	bool		send_json(JsonObject json);

private:


};


#endif
