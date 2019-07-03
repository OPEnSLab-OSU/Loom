#pragma once

#include "Loom_Module.h"


/// Different communication platforms available
enum class CommPlatform {
	WIFI, 		///< WiFi
	LORA, 		///< LoRa
	NRF,		///< Nordic Radio
	BLE,		///< Bluetooth
	SSER 		///< Slip Serial
};


/// Abstract base of communication platform modules
class LoomCommPlat : public LoomModule
{

protected:

	/// The maximum message length
	uint16_t	max_message_len;

	/// Rssi for Lora (need to determine what the other platforms use)
	int16_t		signal_strength;

	StaticJsonDocument<2000> messageJson;

public:

	// Constructor
	LoomCommPlat(
			const char*		module_name,
			uint			max_message_len
		);

	/// Destructor
	virtual ~LoomCommPlat();


	static char*		enum_comm_plat_string(CommPlatform c);
	static CommPlatform string_to_enum_comm_plat(const char* s);


	// General
	virtual void	print_config() override;
	virtual void	measure() {}
	virtual bool	cmd_route(JsonObject) {}
	virtual void 	package(JsonObject json) override;

	virtual void	set_address(uint a) = 0;
	virtual uint	get_address() = 0;


	// Build json from packet if any exists
	virtual bool	receive(JsonObject json) {}

	// Send json
	virtual bool	send(JsonObject json, uint16_t destination) {}
	virtual bool	send(JsonObject json) {}

};


