///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_SpoolPublish.h
/// @brief		File for Loom_SpoolPublish definition.
/// @author		Eli Winkelman, Maddie Smith, and Kenneth Kang
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "PublishPlat.h"
#include "ArduinoJson.h"


///////////////////////////////////////////////////////////////////////////////
///
/// Module taking in and translating JSON into data understood by the Spool API.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___spool_publish.html)
/// - [WiKi: Using Spool with Loom](https://github.com/OPEnSLab-OSU/Spool/wiki)
/// 
///////////////////////////////////////////////////////////////////////////////
class Loom_SpoolPublish : public LoomPublishPlat
{

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Loom Spool Platform module constructor.
	/// Check out https://github.com/OPEnSLab-OSU/Spool/wiki for more information on these parameters.
	/// @param[in]	module_name				String | <"Internet-Plat"> | null | Publish platform module name.
	/// @param[in]  internet_type			Set(LoomModule::Type) | <7001> | {7001("Ethernet"), 7002("WiFi"), 7003("LTE")} | Code of the desired internet platform. 
	/// @param[in]  spool_domain			String | <"device.open-sensing.org"> | device.open-sensing.org | URL where Spool UI is hosted. 
	/// @param[in]  device_data_endpoint	String | <"/api/device/data"> | /api/device/data | URL where device data is sent by the API.
	/// @param[in]  coordinator_id			String | <""> | null | ID for the coordinator device on the network.
	/// @param[in]  device_id				String | <""> | null | ID for the device which will publish data.
	/// @param[in]  data_run				String | <"101"> | 101 | Data run to categorize the sensor model/trial for data collection.
	/// @param[in]  cli_cert				String | <""> | null | Client certificate used to authenticate the SSH client.
	/// @param[in]  cli_key					String | <""> | null | Client key used to authenticate the SSH client.
	//TODO: Need to fill the description from coordinator_id to cli_key.
	Loom_SpoolPublish(
			LoomManager* manager,
			const char*				module_name,
			const LoomModule::Type	internet_type,
			const char*				spool_domain					= "device.open-sensing.org",
			const char*				device_data_endpoint			= "/api/device/data",
			const char*             coordinator_id,
      		const char*             device_id,
			const char* 			data_run						= "101",
			const char* 			cli_cert,
			const char*				cli_key

		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]  p     The array of constuctor args to expand
	Loom_SpoolPublish( LoomManager* manager, JsonArrayConst p );

	/// Destructor
	~Loom_SpoolPublish() = default;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void print_config() const override;

protected:

	/// Send JSON data to a Spool
	/// @param[in] json The JSON data, formatted according to publish();
	/// @param[in] plat A pointer to an internet platform
	/// @return True if success
	/// @warning	Remember to close the socket!
	bool send_to_internet(const JsonObject json, LoomInternetPlat* plat) override;

private:
	
	const String m_spool_domain;			///< The Spool URL
	const String m_device_data_endpoint;
	const String m_coordinator_id;
	const String m_device_id;
	const String m_data_run;

	const std::vector<unsigned char>	m_cli_cert;	///< The client certificate, if one is provided (DER format)
	const std::vector<unsigned char>	m_cli_key;	///< The client private key, if one if provided (DER format)
	br_x509_certificate	m_cert;
	const SSLClientParameters	m_params;
};
