///////////////////////////////////////////////////////////////////////////////
///
/// @file		APWiFi.h
/// @brief		File for APWiFi definition.
/// @author		Luke Goertzen
/// @date		2021
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Module.h"
#include "InternetPlat.h"

#include <WiFi101.h>
#include <WiFiUdp.h>
#undef min
#undef max
#include <memory>

///////////////////////////////////////////////////////////////////////////////
///
/// WiFi InternetPlat
///
/// @par Resources
/// - [Product Page: Adafruit Feather M0 WiFi](https://www.adafruit.com/product/3010)
/// - [Dependency: WiFi101](https://github.com/arduino-libraries/WiFi101)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_APWiFi : public LoomInternetPlat
{

protected:
	
	String	SSID;		///< Host WiFi network name
	String	password;	///< Host WiFi network password

	WiFiServer server;

public:

//==============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================
	
	/// Constructor
	/// @param[in]	ssid	WiFi network name
	/// @param[in]	pass	WiFi network password. Leave as empty string if network has no password.
	Loom_APWiFi(
		LoomManager* manager
	);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_APWiFi(LoomManager* manager, JsonArrayConst p );

	/// Destructor
	virtual ~Loom_APWiFi() = default;

	void second_stage_ctor() override;

	//=============================================================================
	///@name	OPERATION
	/*@{*/ //======================================================================

	/// Create access point
	bool			start_AP();

	virtual void	connect() override { start_AP(); };

	/// Disconnect from the internet
	void 			disconnect() override;

	/// Unlike regular InternetPlats, this just returns if the access point is running
	/// @return True if access point is up, false otherwise
	bool			is_connected() const override;

	void			add_config(JsonObject json) override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	IPAddress		get_ip();

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void			print_config() const override;
	void			print_state() const override;
	
private:

	LoomInternetPlat::UDPPtr open_socket(const uint port) override;

	/// Package IP with ID for MaxMSP implementation
	void			package(JsonObject json) override;

	/// Converts wifi status codes (WL_*) into human readable strings
	static const char* m_wifi_status_to_string(const uint8_t status);

	// Override the following to do nothing
	// An unfortunate side effect of making APWiFi an InternetPlat 
	virtual SSLClient&		get_client() override {}
	virtual const SSLClient& get_client() const override {}
	virtual ClientSession	http_request(const char* domain, const char* url, const char* body, const char* verb) override {}
	virtual ClientSession	connect_to_domain(const char* domain) override {}
	virtual ClientSession	connect_to_ip(const IPAddress& ip, const uint16_t port) override {}
};

