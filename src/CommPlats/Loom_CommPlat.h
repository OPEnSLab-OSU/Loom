#pragma once

#include "Loom_Module.h"


///////////////////////////////////////////////////////////////////////////////


/// Abstract base of communication platform modules
class LoomCommPlat : public LoomModule
{

public:

	/// Different communication platforms available
	enum class Platform {
		WIFI, 		///< WiFi
		LORA, 		///< LoRa
		NRF,		///< Nordic Radio
		BLE,		///< Bluetooth
		SSER 		///< Slip Serial
	};

protected:

	uint16_t	max_message_len;			/// The maximum message length
	int16_t		signal_strength;			/// Rssi for Lora (need to determine what the other platforms use)

	// CommPlatforms need their own JsonDocument because an incoming message
	// can only be deserialized into JsonDocuments, not JsonObjects.
	// And it seemed bad design to pass around references to the LoomManager's
	// internal JsonDocument. 
	// Especially as the LoomManager is intended to be non-mandatory for usage of Loom
	StaticJsonDocument<2000> messageJson;	/// Document to read incoming data into

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomCommPlat(
			const char*		module_name,
			uint			max_message_len
		);

	/// Destructor
	virtual ~LoomCommPlat() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	virtual bool	cmd_route(JsonObject) override {}
	virtual void 	package(JsonObject json) override {};


	// Subclasses don't have to implement the following

	/// Build json from packet if any exists
	/// \param[out]	json	Json object to fill with incoming data
	virtual bool	receive(JsonObject json) {}

	/// Version of send for use with LoomManager.
	/// Accesses Json from LoomManager
	bool			receive();

	/// Send json to a specific address
	/// \param[in]	json			Json package to send
	/// \param[in]	destination		Device to send to
	virtual bool	send(JsonObject json, uint16_t destination) {}

	/// Version of send for use with LoomManager.
	/// Accesses Json from LoomManager
	/// \param[in]	destination		Address of destination device
	bool			send(uint16_t destination);

	/// Broadcast data to all that can receive
	/// \param[in]	json	Json object to send
	virtual void	broadcast(JsonObject json) {};

	/// Version of send for use with LoomManager.
	/// Accesses Json from LoomManager
	void			broadcast();


//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	virtual uint	get_address() = 0;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	virtual void	set_address(uint a) = 0;

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	/// Get string of name associated with communication platform enum
	/// \param[in]	c 	CommPlatform to get c-string of
	/// \return c-string of communication platform
	static const char*	enum_comm_plat_string(Platform c);
	
	/// Get enum val associated with string representing communication platform
	/// \param[in]	s	C-string to get CommPlatform enum of
	/// \return CommPlatform 
	static Platform string_to_enum_comm_plat(const char* s);

protected:

	/// Serialize a JsonObject into a MessagePack buffer.
	/// Also contains buffer memsetting and print statements
	/// \param[in]	json		JsonObject to serialize
	/// \param[out]	buffer		Buffer to fill with MessagePack of json
	/// \param[in]	max_len		Length of buffer
	/// return True if success
	bool	json_to_msgpack_buffer(JsonObjectConst json, char* buffer, int max_len);

	/// Deserialize a MessagePack buffer into a JsonObject.
	/// Also clears the json, contains prints and error checks.
	/// \param[in]	buffer		Buffer to deserialize
	/// \param[out]	json		JsonObject to deserialize into
	/// \param[in]	max_len		Length of buffer
	/// return True if success
	bool	msgpack_buffer_to_json(const char* buffer, JsonObject json);

};







