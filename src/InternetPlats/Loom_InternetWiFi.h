#pragma once

#include "Loom_InternetPlat.h"
#include <WiFi101.h>
#include <WiFiUdp.h>


///////////////////////////////////////////////////////////////////////////////

// ### (LoomModule) | dependencies: [] | conflicts: []
/// InternetPlat built off of SSLClient running over an Ethernet Featherwing. Requires 7KB of free SRAM at runtime to use.
// ###
class Loom_WiFi_I : public LoomInternetPlat
{

protected:

	/// Host WiFi network name
	const String	SSID;
	/// Host WiFi network password
	const String	pass;
	/// SSLClient object for WiFi
	WiFiSSLClient client;

public:

//==============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================
	
	/// Constructor
	Loom_WiFi_I(	
			const char* 	ssid		= "",
			const char* 	pass		= ""
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_WiFi_I( JsonArrayConst p );

	/// Destructor
	virtual ~Loom_WiFi_I() = default;

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
	/// \return True if connect, false otherwise
	bool is_connected() override;

	/// Open a UDP socket for sending and recieving incoming data (WARNING: Be careful about recieving data from an open socket!)
	/// \returns A UDP socket for transmitting and recieving, remember to close the socket when you are done!
	UDPPtr open_socket(const uint port) override;

	void package(JsonObject json) override;


//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void print_config() override;
	void print_state() override;
	
private:

};
