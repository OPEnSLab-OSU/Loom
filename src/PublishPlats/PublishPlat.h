///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_PublishPlat.h
/// @brief		File for LoomPublishPlat definition.
/// @author		Noah Koontz
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Module.h"
#include "../InternetPlats/InternetPlat.h"


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
class LoomPublishPlat : public LoomModule
{

protected:

	/// Pointer to internet platform to use to publish
	LoomInternetPlat*	m_internet;

	/// Type of internet platform used to publish.
	/// Needed because finding the module for m_internet happens in second_stage_ctor(),
	/// rather than the regular constructor.
	LoomModule::Type	internet_type;

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor.
	///
	/// @param[in]	module_name			String | <"Internet-Plat"> | null | Publish platform module name
	/// @param[in]	module_type			Type of the module (provided by derived classes)
	/// @param[in]  internet_type		Set(LoomModule::Type) | <7001> | {7001("Ethernet"), 7002("WiFi"), 7003("LTE")} | Code of the desired internet platform. 
	LoomPublishPlat(
		LoomManager* manager,
		const char*				module_name,
		const LoomModule::Type	module_type,
		const LoomModule::Type	internet_type
	);

	/// Destructor
	virtual ~LoomPublishPlat() = default;

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
	virtual bool send_to_internet(const JsonObject json, LoomInternetPlat* plat) = 0;

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
