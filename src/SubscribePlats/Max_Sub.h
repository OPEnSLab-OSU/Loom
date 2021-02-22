///////////////////////////////////////////////////////////////////////////////
///
/// @file		Max_Sub.h
/// @brief		File for MaxSub definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#if defined(LOOM_INCLUDE_MAX) && (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET))

#pragma once

#include "SubscribePlat.h"

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// Module for receiving data from Max
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___max_sub.html)
/// - [Max 8](https://cycling74.com)
/// - [Loom Max Patches](https://github.com/OPEnSLab-OSU/Max-Loom2)
///
///////////////////////////////////////////////////////////////////////////////
class MaxSub : public SubscribePlat 
{
public:
//=============================================================================
///@name	TYPES
/*@{*/ //======================================================================

	enum class WiFiMode {
		AP, CLIENT, INVALID
	};
protected:
	
	uint16_t	UDP_port;				///< Which UDP port to receive on
	bool		auto_dispatch;			///< True to immediately call Manager::dispatch() when packet received

	InternetPlat::UDPPtr UDP_Inst;		///< Pointer to UDP object

	WiFiMode	wifi_mode;				///< WiFi Mode

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// @param[in]	auto_dispatch		True to immediately call Manager::dispatch() when packet received
	MaxSub(
		const bool auto_dispatch	= true
	); 

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]  p     The array of constuctor args to expand
	MaxSub(JsonArrayConst p);

	void second_stage_ctor() override;

	/// Destructor
	~MaxSub() { if (UDP_Inst) UDP_Inst->stop(); }

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// No package necessary for subscribe platforms.
	/// Implement with empty body.
	void		package(JsonObject json) override { /* do nothing for now */ }	

	bool		subscribe(JsonObject json) override;

	using 		SubscribePlat::subscribe;

	/// Route commands
	virtual bool dispatch(JsonObject json) override;

	/// Go to Access Point WiFi mode
	bool		goto_ap_mode();

	/// Go to Client WiFi mode 
	bool		goto_client_mode(const char* ssid, const char* pass);

	void		add_config(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the UDP port currently receiving on
	/// @return UDP port being used
	uint16_t	get_port() const { return UDP_port; }

	/// Get the last IP address received from
	/// @return Last IP address
	IPAddress	get_remote_IP() const { return (UDP_Inst) ? UDP_Inst->remoteIP() : IPAddress(0,0,0,0); }

	/// Get which WiFi mode is being used
	/// @return WiFi mode
	WiFiMode	get_wifi_mode();

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set the UDP port to receive on.
	/// Changing the port from automatic port setting (based on instance number)
	/// may make the device stop receiving from Max MSP
	/// @param[in]	port	Port to set to
	void		set_port(const uint16_t port);

private:

};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, MaxSub, "MaxSub");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // if defined(LOOM_INCLUDE_MAX) && (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET))
