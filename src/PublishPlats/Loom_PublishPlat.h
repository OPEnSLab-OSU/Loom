#pragma once

#include "Loom_Module.h"
#include "../InternetPlats/Loom_InternetPlat.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomModule) | dependencies: [] | conflicts: []
/// Abstract internet publishing module, implementing google sheets and mongodb functionality
// ###
class LoomPublishPlat : public LoomModule
{

protected:

	LoomInternetPlat*	m_internet;

	LoomModule::Type	internet_type;

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Loom Publish Platform module constructor.
	///
	/// \param[in]	module_name			String | <"Internet-Plat"> | null | Publish platform module name
	/// \param[in]  internet_index		Int | <0> | [0-5] | Index from zero of of the desired internet platform based on the JSON configuration
	LoomPublishPlat( 
		const char*			module_name,
		LoomModule::Type	module_type,
		LoomModule::Type	internet_type
	);

	/// Destructor
	virtual ~LoomPublishPlat() = default;

	/// Grab the internet platform specified by the ctor parameters
	void second_stage_ctor() override;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

    void Run() {}
	void package(JsonObject json) override { /* do nothing for now */ }
	bool dispatch(JsonObject json) override { /* do nothing for now */}

	/// \param[in] json JSON object to publish. MUST be formatted as 
	/// { "contents" : [ { "module": "module_name", "data" : {...} }, ... ], "timestamp"(optional) : {...} }
	/// \returns Whether or not the publish succeded
	bool publish(const JsonObject json);

	/// Version of log for use with LoomManager.
	/// Accesses Json from LoomManager
	bool publish();

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void print_state() override;
	virtual void print_config() override;

protected:

	/// Send JSON to the internet, assumes that a valid internet plat was found and can be used
	/// \param[in]	json	Json object to send
	/// \param[in]	plat	Internet platform to send on
	virtual bool send_to_internet(const JsonObject json, LoomInternetPlat* plat) = 0;

	// Switch to: ?
	// virtual bool send_to_internet(const JsonObject json) = 0;

private:

	/// Check that the JSON supplied meets the format criteria required by publish()
	bool m_validate_json(const JsonObjectConst json);
	/// Print a JSON error
	void m_print_json_error(const char* str);
	
};

