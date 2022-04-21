///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Max_Pub.h
/// @brief		File for MaxPub definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#if defined(LOOM_INCLUDE_MAX) && (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET))
#pragma once

#include "PublishPlat.h"

namespace Loom {

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
class MaxPub : public PublishPlat
{

protected:

	uint16_t	UDP_port;				///< Which UDP port to transmit on
	IPAddress	remoteIP;				///< Which IP address to send to

	InternetPlat::UDPPtr UDP_Inst;	///< Pointer to UDP object

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	MaxPub();

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]  p	The array of constuctor args to expand
	MaxPub(JsonArrayConst p );

	void second_stage_ctor() override;

	/// Destructor
	~MaxPub() { if (UDP_Inst) UDP_Inst->stop(); }

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

	bool		send_to_internet(const JsonObject json, InternetPlat* plat) override;

private:

};

///////////////////////////////////////////////////////////////////////////////
REGISTER_NODEFAULT(Module, MaxPub, "MaxPub");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // if defined(LOOM_INCLUDE_MAX) && (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET))
