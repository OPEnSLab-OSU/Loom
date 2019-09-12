///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_SpoolPublish.h
/// @brief		File for Loom_SpoolPublish definition.
/// @author		Eli Winkelman
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Loom_PublishPlat.h"
#include "ArduinoJson.h"


///////////////////////////////////////////////////////////////////////////////
///
/// Loom Spool publishing platform.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___spool_publish.html)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_SpoolPublish : public LoomPublishPlat 
{
	
public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	Loom_SpoolPublish(
			const char* module_name,
			const LoomModule::Type	internet_type,
			const char* spool_domain,
			const char* device_data_endpoint,
			const char* device_id
		);
	
	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]  p     The array of constuctor args to expand
	Loom_SpoolPublish( JsonArrayConst p );
	
	/// Destructor
	~Loom_SpoolPublish() = default;
	
//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void print_config() override;
	
protected:
	
	/// @warning	Remember to close the socket!
	bool send_to_internet(const JsonObject json, LoomInternetPlat* plat) override;
	
private:

	const String m_spool_domain;			///< The Spool URL
	const String m_device_data_endpoint;
	const String m_device_id;

};
