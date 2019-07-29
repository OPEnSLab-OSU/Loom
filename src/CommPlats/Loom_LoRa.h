#pragma once

#include "Loom_CommPlat.h"

#include <RH_RF95.h>
#include <RHReliableDatagram.h>

// LoRa Chip pins
#define RFM95_CS  8		/// LoRa hip select pin
#define RFM95_RST 4		/// LoRa reset pin
#define RFM95_INT 3		/// LoRa interrupt pin

/// LoRa radio frequence.
/// Hardware specific, Tx must match Rx
#define RF95_FREQ 915.0


///////////////////////////////////////////////////////////////////////////////


// ### (LoomCommPlat) | dependencies: [] | conflicts: []
/// LoRa communication platform module
// ###
class Loom_LoRa : public LoomCommPlat
{

protected:

	RH_RF95*			driver;				///	Underlying rf95 object
	RHReliableDatagram*	manager;			/// Manager for driver

	uint8_t				address;			/// Device Address    (should this be part of LoomCommPlat? – maybe not as each platform handles addresses differently)

	uint8_t				power_level;		/// Power level to send at
	uint8_t				retry_count;		/// Number of transmission retries allowed
	uint16_t			retry_timeout;		/// Delay between transmission retries (in milliseconds)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

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
			uint8_t			address				= 0,
			uint8_t			power_level 		= 23,
			uint8_t			retry_count			= 3,
			uint16_t		retry_timeout		= 200
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_LoRa(JsonArrayConst p);

	/// Destructor
	~Loom_LoRa();

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	bool		receive(JsonObject json) override;
	bool		send(JsonObject json, uint16_t destination) override;

	// manually expose superclass version of log() that gets json from
	// linked LoomManager, calling this classes implementation of 
	// 'send(JsonObject)' and 'send(JsonObject, uint16_t)', which is pure virtual in superclass
	using LoomCommPlat::send; 
	using LoomCommPlat::receive; 
	using LoomCommPlat::broadcast; 


//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	uint		get_address() override;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	void		set_address(uint addr) override;

private:

};


