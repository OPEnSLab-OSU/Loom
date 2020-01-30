///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_nRF.h
/// @brief		File for Loom_nRF definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "CommPlat.h"

#include <RF24Network.h>
#include <RF24.h>


#define NRF_MESSAGE_SIZE 120	///< Max nRF message size


///////////////////////////////////////////////////////////////////////////////
///
/// Nordic Radio communication platform module
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom__n_r_f.html)
/// - [Dependency: RF24](https://github.com/nRF24/RF24) optimized fork
/// - [Dependency: RF24Network](https://github.com/nRF24/RF24Network) optimized fork
///
///////////////////////////////////////////////////////////////////////////////
class Loom_nRF : public LoomCommPlat
{

protected:

	RF24				radio;				///< Underlying nRF24L01(+) radio object
	RF24Network*		network;			///< Network layer manager for radio

	uint8_t				address;			///< Device Address    (should this be part of LoomCommPlat? – maybe not as each platform handles addresses differently)

	uint8_t				data_rate;			///< Data rate
	uint8_t				power_level;		///< Power level to send at
	uint8_t				retry_count;		///< Number of transmission retries allowed
	uint16_t			retry_timeout;		///< Delay between transmission retries (in milliseconds)

	uint8_t				multicast_level;	///< Which level(s) of the network to broadcast to

//=============================================================================
///@name	RADIO IMPLEMENTATION
/*@{*/ //======================================================================

	/// Receive, but block until packet received, or timeout reached
	/// @param[out]	json			Json object to fill with incoming data
	/// @param[in]	max_wait_time	Maximum number of milliseconds to block for (can be zero for non-blocking)
	/// @return True if packet received
	bool receive_blocking_impl(JsonObject json, uint max_wait_time) override;

	/// Send json to a specific address
	/// @param[in]	json			Json package to send
	/// @param[in]	destination		Device to send to
	/// @return True if packet sent successfully
	bool send_impl(JsonObject json, const uint8_t destination) override;

	/// Broadcast data to all that can receive.
	/// Derived classes can optionally provide an implementation for this,
	/// As supported by the radio/platform's library
	/// @param[in]	json	Json object to send
	void broadcast_impl(JsonObject json) override;

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// nRF module constructor
	///
	/// @param[in]	max_message_len		Set(Int) | <120> | {120("Max length")} | The maximum possible message length
	/// @param[in]	address 			Int | <01> | [0-99] | This device's nRF address
	/// @param[in]	data_rate			Set(Int) | <0> | {0("Default"), 1("250KBPS"), 2("1MBPS"), 3("2MBPS")} | Transmission data rate
	/// @param[in]	power_level			Set(Int) | <0> | {0("Default"), 1("Min"), 2("Low"), 3("High"), 4("Max")} | Transmission ower level
	/// @param[in]	retry_count 		Int | <3> | [0-15] | Max number of transmission retries
	/// @param[in]	retry_timeout 		Int | <200> | [20-500] | Delay between retransmissions (ms)
	/// @param[in]	multicast_level		Int | <1> | [1-3] | How many levels to propogate message through heirarchy
	Loom_nRF(
			LoomManager* manager,
			const uint16_t		max_message_len		= 120,
			const uint8_t		address 			= 0,
			const uint8_t		data_rate			= 1,
			const uint8_t		power_level			= 0,
			const uint8_t		retry_count 		= 3,
			const uint16_t		retry_timeout 		= 200,
			const uint8_t		multicast_level		= 1
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_nRF(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_nRF();

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		add_config(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	uint8_t		get_address() const override { return address; }

	/// Get multicast(broadcast) level.
	/// nRF has varying degrees of broadcast corresponding to 
	/// depth to broadcast through network tree
	/// @return Multicast level
	uint8_t		get_multicast_level() const { return multicast_level; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	void		set_address(const uint8_t addr) override;

	/// Set multicast(broadcast) level.
	/// nRF has varying degrees of broadcast corresponding to 
	/// depth to broadcast through network tree
	/// @param[in]	level 	Multicast level to set to
	void		set_multicast_level(const uint8_t level) { multicast_level = level; }

private:

};


