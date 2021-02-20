///////////////////////////////////////////////////////////////////////////////
///
/// @file		InternetEthernet.h
/// @brief		File for Ethernet definition.
/// @author		Noah Koontz
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_ETHERNET
#pragma once

#include "InternetPlat.h"

#include <EthernetLarge.h>
#include <SSLClient.h>

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// InternetPlat built off of SSLClient running over an Ethernet Featherwing.
/// 
/// @attention	Requires 7KB of free SRAM at runtime to use.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___ethernet.html)
/// - [Product Page: Adafruit Ethernet Featherwing](https://www.adafruit.com/product/3201)
/// - [Dependency: EthernetLarge](https://github.com/OPEnSLab-OSU/EthernetLarge) OPEbS Lab fork of Arduino Ethernet library
/// - [Dependency: SSLClient](https://github.com/OPEnSLab-OSU/SSLClient)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#ethernet)
///
///////////////////////////////////////////////////////////////////////////////
class Ethernet : public InternetPlat
{

protected:

	EthernetClient m_base_client;
	SSLClient m_client;		///< Underlying Ethernet SSLclient instance

	byte			m_mac[6];				///< The Ethernet MAC address
	IPAddress		m_ip;					///< The devices IP address
	
	bool			m_is_connected;			///< Whether or not ethernet initialized successfully
	
	SSLClient& get_client() override { return m_client; }
	const SSLClient& get_client() const { return m_client; }

public:
	
//==============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	Ethernet(	
			const char* 			module_name	= "Ethernet",
			const JsonArrayConst	mac			= JsonArray(),
			const JsonArrayConst	ip			= JsonArray()
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Ethernet(JsonArrayConst p );

	/// Destructor
	virtual ~Ethernet() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================
	/// Connect to internet
	void connect() override;

	/// Whether or not connected to internet
	/// @return True if connect, false otherwise
	bool is_connected() const override { m_is_connected; }

	/// Open a UDP socket for sending and recieving incoming data.
	/// @warning Be careful about recieving data from an open socket!
	/// @param[in]	port	Port to open socket with
	/// @returns A UDP socket for transmitting and recieving, remember to close the socket when you are done!
	UDPPtr open_socket(const uint port) override;

	void add_config(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;
	void		print_state() const override;

private:

};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, Ethernet, "Ethernet");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_ETHERNET
