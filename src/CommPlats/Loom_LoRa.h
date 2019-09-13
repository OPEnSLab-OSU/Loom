///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_LoRa.h
/// @brief		File for Loom_LoRa definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Loom_CommPlat.h"

#include <RH_RF95.h>
#include <RHReliableDatagram.h>

// LoRa Chip pins
#define RFM95_CS  8		//< LoRa hip select pin
#define RFM95_RST 4		///< LoRa reset pin
#define RFM95_INT 3		///< LoRa interrupt pin

/// LoRa radio frequence.
/// Hardware specific, Tx must match Rx
#define RF95_FREQ 915.0


///////////////////////////////////////////////////////////////////////////////
///
/// LoRa communication platform module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___lo_ra.html)
/// - [Product Page: Adafruit Feather M0 LoRa](https://www.adafruit.com/product/3178)
/// - [Documentation: RadioHead Library](https://www.airspayce.com/mikem/arduino/RadioHead/index.html)
/// - [Dependency: RadioHead](https://github.com/adafruit/RadioHead)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_LoRa : public LoomCommPlat
{

protected:

	RH_RF95				driver;			///< Underlying rf95 object
	RHReliableDatagram*	manager;		///< Manager for driver

	uint8_t				address;		///< Device Address    (should this be part of LoomCommPlat? – maybe not as each platform handles addresses differently)

	uint8_t				power_level;	///< Power level to send at
	uint8_t				retry_count;	///< Number of transmission retries allowed
	uint16_t			retry_timeout;	///< Delay between transmission retries (in milliseconds)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// LoRa module constructor
	///
	/// @param[in]	max_message_len				Set(Int) | <255> | {255("Max length")} | The maximum possible message length
	/// @param[in]	address						Int | <01> | [0-99] | This device's LoRa address
	/// @param[in]	power_level					Int | <23> | [5-23] | Transmission power level, low to high power
	/// @param[in]	retry_count					Int | <3> | [0-15] | Max number of transmission retries
	/// @param[in]	retry_timeout				Int | <200>| [20-500] | Delay between retransmissions (ms)
	Loom_LoRa(
			const uint16_t		max_message_len		= RH_RF95_MAX_MESSAGE_LEN,
			const uint8_t		address				= 0,
			const uint8_t		power_level 		= 23,
			const uint8_t		retry_count			= 3,
			const uint16_t		retry_timeout		= 200
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_LoRa(JsonArrayConst p);

	/// Destructor
	~Loom_LoRa();

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	bool		receive(JsonObject json) override;
	bool		send(JsonObject json, const uint8_t destination) override;
	void		add_config(JsonObject json) override;

	// manually expose superclass version of log() that gets json from
	// linked LoomManager, calling this classes implementation of 
	// 'send(JsonObject)' and 'send(JsonObject, uint8_t)', which is pure virtual in superclass
	using LoomCommPlat::send; 
	using LoomCommPlat::receive; 
	using LoomCommPlat::broadcast; 


//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	uint8_t		get_address() const override { return address; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	void		set_address(const uint8_t addr) override;

private:

};


