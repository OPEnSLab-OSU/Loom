
#ifndef LOOM_COMM_PLAT_h
#define LOOM_COMM_PLAT_h


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

	// Not sure if address will be in this scope of only the individual implementations

public:

	// --- CONSTRUCTOR ---
	LoomCommPlat(
			const char*		module_name,
			uint			max_message_len
		);

	// --- DESTRUCTOR ---
	virtual ~LoomCommPlat();


	static char*		enum_comm_plat_string(CommPlatform c);
	static CommPlatform string_to_enum_comm_plat(const char* s);


	// General
	virtual void	print_config() override;
	virtual void	measure() {}
	virtual bool	message_route(OSCMessage& msg, int address_offset) {}
	virtual void 	package(JsonObject json) override;


	// Build OSC Bundle from packet if any exists
	virtual bool	receive_bundle(OSCBundle& bndl) = 0;

	// Send an OSC bundle
	// Should overload or use default args to optionally allow default address
	virtual bool	send_bundle(OSCBundle& bndl, uint16_t destination) = 0;
	virtual bool	send_bundle(OSCBundle& bndl) = 0;

	// Broadcast an OSC bundle
	virtual void	broadcast_bundle(OSCBundle& bndl) = 0;

	virtual void	set_address(uint a) = 0;
	virtual uint	get_address() = 0;


	// Build json from packet if any exists
	virtual bool	receive_json(JsonObject json) {}

	// Send json
	virtual bool	send_json(JsonObject json, uint16_t destination) {}
	virtual bool	send_json(JsonObject json) {}



protected:


	/// Convert a c string to equivalent bundle.
	/// Calls aux function to uncompress message if neccessary
	/// \param[in]	osc_string	The string to convert
	/// \param[out]	bndl		The bundle to fill
	void			convert_string_to_bundle(char* osc_string, OSCBundle& bndl);

	/// Convert a bundle to equivalent c string
	/// Calls aux function to try to compress message
	/// \param[out]	bndl		The bundle to convert
 	/// \param[in]	osc_string	The string to fill
	void			convert_bundle_to_string(OSCBundle& bndl, char* osc_string);


private:

	/// Conversion from c string to bundle without compression
	/// \param[in]	osc_string	String to convert
	/// \param[out]	bndl		Bundle to fill
	void			original_convert_string_to_bundle(char* osc_string, OSCBundle& bndl);

	/// Conversion from bundle to string without decompression
	/// \param[in]	bndl		Bundle to convert
	/// \param[out]	osc_string	String buffer to fill
	void			original_convert_bundle_to_string(OSCBundle& bndl, char* osc_string);


};




#endif
