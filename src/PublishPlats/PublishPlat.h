///////////////////////////////////////////////////////////////////////////////
///
/// @file		PublishPlat.h
/// @brief		File for PublishPlat definition.
/// @author		Noah Koontz
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
#pragma once

#include "Module.h"
#include "../InternetPlats/InternetPlat.h"
#include "../LogPlats/BatchSD.h"

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// Abstract internet publishing module, implementing google sheets and mongodb functionality
///
/// All actuator modules inherit from this class.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_publish_plat.html)
///
///////////////////////////////////////////////////////////////////////////////
class PublishPlat : public Module
{

protected:

	/// Pointer to internet platform to use to publish
	InternetPlat*	m_internet;

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor.
	///
	/// @param[in]	module_name			String | <"Internet-Plat"> | null | Publish platform module name
	PublishPlat(
		const char*				module_name
	);

	/// Destructor
	virtual ~PublishPlat() = default;

	/// Grab the internet platform specified by the ctor parameters
	void second_stage_ctor() override;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// No package necessary for publishing platforms.
	/// Implement with empty body.
	void	package(JsonObject json) override { /* do nothing for now */ }

	/// Publish data.
	/// @param[in] json JSON object to publish. MUST be formatted as
	/// 	{ "contents" : [ { "module": "module_name", "data" : {...} }, ... ], "timestamp"(optional) : {...} }
	/// @returns Whether or not the publish succeded
	bool	publish(const JsonObject json);

	/// Publish all the packets stored in the batch
	/// @returns Whether or not the publishes succeded or not
	uint8_t publish_batch();

	/// Version of log for use with LoomManager.
	/// Accesses Json from LoomManager
	/// @return True if success
	bool	publish();

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_state() const override;
	virtual void	print_config() const override;

protected:

	/// Send JSON to the internet, assumes that a valid internet plat was found and can be used
	/// @param[in]	json	Json object to send
	/// @param[in]	plat	Internet platform to send on
	/// @return True if success
	virtual bool send_to_internet(const JsonObject json, InternetPlat* plat) = 0;

	// Switch to: ?
	// virtual bool send_to_internet(const JsonObject json) = 0;

private:

	/// Check that the JSON supplied meets the format criteria required by publish()
	/// @param[in]	json	Json to validate
	bool m_validate_json(const JsonObjectConst json) const;

	/// Print a JSON error
	/// @param[in]	str		Error string to print
	void m_print_json_error(const char* str) const;

};

///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
