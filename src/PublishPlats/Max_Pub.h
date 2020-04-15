///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Max_Pub.h
/// @brief		File for Loom_MaxPub definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "PublishPlat.h"


#define UDP_SEND_OFFSET 8000	///< UDP sending port is this value + device instance number


///////////////////////////////////////////////////////////////////////////////
///
/// Module taking in and translating JSON into data understood by the Google Sheets script API.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___max_pub.html)
/// - [Max 8](https://cycling74.com)
/// - [Loom Max Patches](https://github.com/OPEnSLab-OSU/Max-Loom2)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_MaxPub : public LoomPublishPlat
{

protected:

	uint16_t	UDP_port;				///< Which UDP port to transmit on
	IPAddress	remoteIP;				///< Which IP address to send to

	LoomInternetPlat::UDPPtr UDP_Inst;	///< Pointer to UDP object

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]  internet_type	Set(LoomModule::Type) | <7001> | {7001("Ethernet"), 7002("WiFi"), 7003("LTE")} | Code of the desired internet platform.
	Loom_MaxPub(
		LoomManager* manager,
		const LoomModule::Type		internet_type
	);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]  p	The array of constuctor args to expand
	Loom_MaxPub(LoomManager* manager, JsonArrayConst p );

	void second_stage_ctor() override;

	/// Destructor
	~Loom_MaxPub() { if (UDP_Inst) UDP_Inst->stop(); }

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	bool		dispatch(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get UDP port device is listening on
	/// @return UDP port
	uint16_t	get_port() const { return UDP_port; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set the UDP port to send on
	/// @param[in]	port	The UDP port to send on
	void		set_port(const uint16_t port);

	/// Set the IP address to send to
	/// @param[in]	ip	The IP address to set to
	void		set_ip(const IPAddress ip);

	/// Set the IP addres to send to by getting remote IP from MaxSub if available
	void		set_ip();

protected:

	bool		send_to_internet(const JsonObject json, LoomInternetPlat* plat) override;

private:

};
