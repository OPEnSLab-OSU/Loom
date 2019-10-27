///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_InternetEthernet.h
/// @brief		File for Loom_Ethernet definition.
/// @author		Noah Koontz
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Loom_InternetPlat.h"
#include "EthernetLarge.h"
#include "SSLClient.h"


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
///
///////////////////////////////////////////////////////////////////////////////
class Loom_Ethernet : public LoomInternetPlat
{
	static bool Loom_Ethernet_Registered;
protected:

	std::vector<unsigned char>	m_cli_cert;	///< The client certificate, if one is provided (DER format)
	std::vector<unsigned char>	m_cli_key;	///< The client private key, if one if provided (DER format)
	const br_x509_certificate	m_cert;
	const SSLClientParameters	m_params;

	SSLClient<EthernetClient> m_client;		///< Underlying Ethernet SSLclient instance

	byte			m_mac[6];				///< The Ethernet MAC address
	IPAddress		m_ip;					///< The devices IP address
	
	bool			m_is_connected;			///< Whether or not ethernet initialized successfully
	
public:
	
//==============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	Loom_Ethernet(	
			const char* 			module_name	= "Ethernet",
			const JsonArrayConst	mac			= JsonArray(),
			const JsonArrayConst 	ip			= JsonArray(),
			const char*				cli_cert	= nullptr,
			const char*				cli_key		= nullptr
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_Ethernet( JsonArrayConst p );

	/// Destructor
	virtual ~Loom_Ethernet() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	// remember to close the socket!
	ClientSession connect_to_domain(const char* domain) override;

	// remember to close the socket!
	ClientSession connect_to_ip(const IPAddress& ip, const uint16_t port) override;

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

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;
	void		print_state() const override;
};

