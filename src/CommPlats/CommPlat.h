///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_CommPlat.h
/// @brief		File for LoomCommPlat definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Module.h"

///////////////////////////////////////////////////////////////////////////////
///
/// Abstract base of communication platform modules
///
/// All communication platform modules inherit from this class.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_comm_plat.html)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#telecommunication-capabilities)
///
///////////////////////////////////////////////////////////////////////////////
class LoomCommPlat : public LoomModule
{

protected:

	uint16_t	max_message_len;	///< The maximum message length
	int16_t		signal_strength;	///< RSSI for Lora (need to determine what the other platforms use)

	/// Document to read incoming data into.
	/// CommPlatforms need their own JsonDocument because an incoming message
	/// can only be deserialized into JsonDocuments, not JsonObjects.
	/// And it seemed bad design to pass around references to the LoomManager's
	/// internal JsonDocument. 
	/// Especially as the LoomManager is intended to be non-mandatory for usage of Loom
	StaticJsonDocument<1500> messageJson;

	// counters for determining packet drop rate
	// used only for debug
	uint32_t total_packet_count;
	uint32_t total_drop_count;
	bool last_ten_dropped[10];
	uint8_t last_ten_dropped_idx;

//=============================================================================
///@name	RADIO IMPLEMENTATION
/*@{*/ //======================================================================

	/// Receive, but block until packet received, or timeout reached
	/// @param[out]	json			Json object to fill with incoming data
	/// @param[out]	max_wait_time	Maximum number of milliseconds to block for (can be zero for non-blocking)
	/// @return True if packet received
	virtual bool receive_blocking_impl(JsonObject json, uint max_wait_time) {}

	/// Send json to a specific address
	/// @param[in]	json			Json package to send
	/// @param[in]	destination		Device to send to
	/// @return True if packet sent successfully
	virtual bool send_impl(JsonObject json, const uint8_t destination) {}

	/// Broadcast data to all that can receive.
	/// Derived classes can optionally provide an implementation for this,
	/// As supported by the radio/platform's library
	/// @param[in]	json	Json object to send
	virtual void broadcast_impl(JsonObject json) {}

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// @param[in]	module_name		Name of the module (provided by derived classes)
	/// @param[in]	module_type		Type of the module (provided by derived classes)
	/// @param[in]	max_message_len	The maximum possible message length
	LoomCommPlat(
			LoomManager* manager,
			const char* module_name,
			const LoomModule::Type	module_type,
			const uint16_t			max_message_len
		);

	/// Destructor
	virtual ~LoomCommPlat() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	virtual void 	package(JsonObject json) override {};

	/// Receive, but block until packet received, or timeout reached
	/// @param[out]	json			Json object to fill with incoming data
	/// @param[in]	max_wait_time	Maximum number of milliseconds to block for
	/// @return True if packet received
	bool			receive_blocking(JsonObject json, uint max_wait_time);

	/// Build json from packet if any exists
	/// @param[out]	json	Json object to fill with incoming data
	bool			receive(JsonObject json) { return receive_blocking(json, 0); }

	/// Version of receive for use with LoomManager.
	/// Accesses Json from LoomManager
	/// @return True if packet received
	bool			receive();

	/// Version of receive_blocking for use with LoomManager.
	/// Accesses Json from LoomManager
	/// @param[out]	max_wait_time	Maximum number of milliseconds to block for
	/// @return True if packet received	
	bool			receive_blocking(const uint max_wait_time);

	/// Send json to a specific address
	/// @param[in]	json			Json package to send
	/// @param[in]	destination		Device to send to
	/// @return True if packet sent successfully
	bool			send(JsonObject json, const uint8_t destination);

	/// Version of send for use with LoomManager.
	/// Accesses Json from LoomManager
	/// @param[in]	destination		Address of destination device
	/// @return True if packet sent successfully, false otherwise
	bool			send(const uint8_t destination);

	/// Broadcast data to all that can receive.
	/// Derived classes can optionally provide an implementation for this,
	/// As supported by the radio/platform's library
	/// @param[in]	json	Json object to send
	void			broadcast(JsonObject json) { broadcast_impl(json); }

	/// Version of send for use with LoomManager.
	/// Accesses Json from LoomManager
	void			broadcast();

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() const override;
	virtual void	print_state() const override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the address of this device.
	/// Each platform may have a different addressing scheme
	/// @return The address of this device
	virtual uint8_t	get_address() const = 0;

	/// Get the packet drop rate since the start of operation.
	/// Keep in mind that this drop rate will not account for retransmissions
	/// done internally by RadioHead (ex Reliable datagram), but rather will
	/// count data packets that were discarded. This rate will also ignore
	/// broadcasts, since they are nearly impossible to quantify.
	/// @return The drop rate from 0 (no drops) to 100 (100% drop)
	float get_drop_rate() const;

	/// Get the packet drop rate of the last ten send() calls.
	/// Keep in mind that this drop rate will not account for retransmissions
	/// done internally by RadioHead (ex Reliable datagram), but rather will
	/// count data packets that were discarded. This rate will also ignore
	/// broadcasts, since they are nearly impossible to quantify.
	/// @return The drop rate from 0 (no drops) to 100 (100% drop)
	float get_last_ten_drop_rate() const;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set the address of this device.
	/// Each platform may have a different addressing scheme.
	/// Changing the device's address is not recommended.
	/// @param[in]	a 	The address to set this device to
	virtual void	set_address(const uint8_t a) = 0;

protected:

	/// Serialize a JsonObject into a MessagePack buffer.
	/// Also contains buffer memsetting and print statements
	/// @param[in]	json		JsonObject to serialize
	/// @param[out]	buffer		Buffer to fill with MessagePack of json
	/// @param[in]	max_len		Length of buffer
	/// @return True if success
	bool	json_to_msgpack_buffer(JsonObjectConst json, char* buffer, const uint16_t max_len) const;

	/// Deserialize a MessagePack buffer into a JsonObject.
	/// Also clears the json, contains prints and error checks.
	/// @param[in]	buffer		Buffer to deserialize
	/// @param[out]	json		JsonObject to deserialize into
	/// @param[in]	max_len		Length of buffer
	/// @return True if success
	bool	msgpack_buffer_to_json(const char* buffer, JsonObject json);

	/// Add the result of a packet to the drop_rate tracker
	/// @param[in] did_drop		Whether or not the packet dropped during transmission.
	void	add_packet_result(const bool did_drop);

};







