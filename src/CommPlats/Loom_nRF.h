
#ifndef LOOM_NRF_h
#define LOOM_NRF_h

#include "Loom_CommPlat.h"

#include <RF24Network.h>
#include <RF24.h>


// // these will be defined in the config file
#define NRF_SELF_ADDRESS   01
#define NRF_FRIEND_ADDRESS 00

/// Max nRF message size
#define NRF_MESSAGE_SIZE 120

// ### (LoomCommPlat) | dependencies: [] | conflicts: []
/// Nordic Radio communication platform module
// ###
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

	/// nRF module constructor
	///
	/// \param[in]	module_name				String | <"nRF"> | null | nRF module name
	/// \param[in]	max_message_len		Set(Int) | <120> | {120("Max length")} | The maximum possible message length
	/// \param[in]	compress_messages	Bool | <true> | {true, false} | Whether or not to try to compress messages
	/// \param[in]	address 					Int | <01> | [0-99] | This device's nRF address
	/// \param[in]	friend_address 		Int | <00> | [0-99] | Default nRF address to send to
	/// \param[in]	data_rate					Set(Int) | <0> | {0("Default"), 1("250KBPS"), 2("1MBPS"), 3("2MBPS")} | Transmission data rate
	/// \param[in]	power_level				Set(Int) | <0> | {0("Default"), 1("Min"), 2("Low"), 3("High"), 4("Max")} | Transmission ower level
	/// \param[in]	retry_count 			Int | <3> | [0-15] | Max number of transmission retries
	/// \param[in]	retry_timeout 		Int | <200> | [20-500] | Delay between retransmissions (ms)
	/// \param[in]	multicast_level		Int | <1> | [1-3] | How many levels to propogate message through heirarchy
	Loom_nRF(
			const char*		module_name			= "nRF",
			uint16_t		max_message_len		= 120,
			bool			compress_messages	= true,
			uint8_t			address 			= NRF_SELF_ADDRESS,
			uint8_t			friend_address 		= NRF_FRIEND_ADDRESS,
			uint8_t			data_rate			= 1,
			uint8_t			power_level			= 0,
			uint8_t			retry_count 		= 3,
			uint16_t		retry_timeout 		= 200,
			uint8_t			multicast_level		= 1
		);

	Loom_nRF(JsonVariant p);


	// --- DESTRUCTOR ---
	virtual ~Loom_nRF();


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

	void		set_multicast_level(uint8_t level);
	uint8_t		get_multicast_level();

private:


};


#endif // of #ifndef LOOM_NRF_h
