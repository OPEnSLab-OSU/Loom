#pragma once

#include "Loom_InternetPlat.h"
#include "EthernetLarge.h"
#include "SSLClient.h"


///////////////////////////////////////////////////////////////////////////////


class Loom_Ethernet_I : public LoomInternetPlat
{

protected:
	
	SSLClient<EthernetClient> m_client;		/// Underlying Ethernet SSLclient instance
	EthernetUDP 	m_UDP;					/// Underlying Ethernet UDP instance
	
	byte 			m_mac[6];				/// The Ethernet MAC address
	IPAddress 		m_ip;					/// The devices IP address
	
	bool m_is_connected;					/// Whether or not ethernet initialized successfully
	
public:
	
//============================s=================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	Loom_Ethernet_I(	
			const char* 			module_name		= "Ethernet",
			const JsonArrayConst	mac				= JsonArray(),
			const JsonArrayConst 	ip				= JsonArray()
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_Ethernet_I( JsonArrayConst p );

	/// Destructor
	~Loom_Ethernet_I() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	// remember to close the socket!
	Client& http_request(const char* domain, const char* url, const char* body, const char* verb) override;

	/// Connect to internet
	void connect() override;

	/// Whether or not connected to internet
	/// \return True if connect, false otherwise
	bool is_connected() override;

	/// Get UTC time
	/// \return Time if found, 0 otherwise
	uint32_t get_time() override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void print_config() override;
	void print_state() override;

private:

	void m_send_NTP_packet(byte packet_buffer[]);

};

