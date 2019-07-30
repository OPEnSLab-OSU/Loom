#pragma once

#include "Loom_Module.h"
#include "../InternetPlats/Loom_InternetPlat.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomModule) | dependencies: [] | conflicts: []
/// Abstract internet publishing module, implementing google sheets and mongodb functionality
// ###
class LoomSubscribePlat : public LoomModule
{

protected:

	LoomInternetPlat*	internetPlat;
	
	const uint8_t		internet_index;


	// Subscribe Platforms need their own JsonDocument because an incoming message
	// can only be deserialized into JsonDocuments, not JsonObjects.
	// And it seemed bad design to pass around references to the LoomManager's
	// internal JsonDocument. 
	// Especially as the LoomManager is intended to be non-mandatory for usage of Loom
	StaticJsonDocument<1000> messageJson;	/// Document to read incoming data into

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Loom Subscribe Platform module constructor.
	///
	/// \param[in]	module_name			String | <"Internet-Plat"> | null | Subscribe platform module name
	/// \param[in]  internet_index		Int | <0> | [0-5] | Index from zero of of the desired internet platform based on the JSON configuration
	LoomSubscribePlat( 
		const char*			module_name,
		LoomModule::Type	module_type
		// const uint8_t		internet_index
	);

	/// Destructor
	virtual ~LoomSubscribePlat() = default;

	/// Grab the internet platform specified by the ctor parameters
	void second_stage_ctor() override;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		package(JsonObject json) override { /* do nothing for now */ }
	bool		dispatch(JsonObject json) override { /* do nothing for now */}

	virtual bool subscribe(const JsonObject json) = 0;

	/// Version of log for use with LoomManager.
	/// Accesses Json from LoomManager
	bool		subscribe();


//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	// virtual void print_state();
	// virtual void print_config();

protected:

	/// Deserialize a MessagePack buffer into a JsonObject.
	/// Also clears the json, contains prints and error checks.
	/// \param[in]	buffer		Buffer to deserialize
	/// \param[out]	json		JsonObject to deserialize into
	/// \param[in]	max_len		Length of buffer
	/// return True if success
	bool msgpack_buffer_to_json(const char* buffer, JsonObject json);

};

