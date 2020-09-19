///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_InternetWifi.h
/// @brief		File for WiFi definition.
/// @author		Noah Koontz
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_WIFI
#pragma once

#include "InternetPlat.h"
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <SSLClient.h>

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// WiFi InternetPlat
///
/// @attention	Requires 7KB of free SRAM at runtime to use.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___wifi.html)
/// - [Product Page: Adafruit Feather M0 WiFi](https://www.adafruit.com/product/3010)
/// - [Dependency: WiFi201](https://github.com/OPEnSLab-OSU/WiFi201) OPEbS Lab fork of Arduino WiFi101 library
/// - [Dependency: WiFi101](https://github.com/arduino-libraries/WiFi101)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#wifi)
///
///////////////////////////////////////////////////////////////////////////////
class L_WiFi : public InternetPlat
{

protected:

	const String	SSID;	///< Host WiFi network name
	const String	pass;	///< Host WiFi network password

	WiFiClient m_base_client;	///< SSLClient object for WiFi
	SSLClient m_client;		///< Underlying Wifi SSLclient instance

	SSLClient& get_client() override { return m_client; }
	const SSLClient& get_client() const override { return m_client; }

public:

//==============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// @param[in]	ssid	WiFi network name
	/// @param[in]	pass	WiFi network password. Leave as empty string if network has no password.
	L_WiFi(
			const char* 	ssid	= "",
			const char* 	pass	= ""
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	L_WiFi(JsonArrayConst p );

	/// Destructor
	virtual ~L_WiFi() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// Connect to internet
	void			connect() override;

	/// Disconnect from the internet
	void 			disconnect() override;

	/// Whether or not connected to internet
	/// @return True if connect, false otherwise
	bool			is_connected() const override;

	/// Open a UDP socket for sending and recieving incoming data.
	/// @warning Be careful about recieving data from an open socket!
	/// @returns A UDP socket for transmitting and recieving, remember to close the socket when you are done!
	UDPPtr			open_socket(const uint port) override;

	/// Package IP with ID for MaxMSP implementation
#ifdef LOOM_INCLUDE_MAX
	void			package(JsonObject json) override;
#endif

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void			print_config() const override;
	void			print_state() const override;

private:

	/// Converts wifi status codes (WL_*) into human readable strings
	static const char* m_wifi_status_to_string(const uint8_t status);
};

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_WIFI
