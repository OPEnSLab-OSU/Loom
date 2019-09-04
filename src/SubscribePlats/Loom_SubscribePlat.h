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

	LoomInternetPlat*	m_internet;
	
	LoomModule::Type	internet_type;


	// Subscribe Platforms need their own JsonDocument because an incoming message
	// can only be deserialized into JsonDocuments, not JsonObjects.
	// And it seemed bad design to pass around references to the LoomManager's
	// internal JsonDocument. 
	// Also as the LoomManager is intended to be non-mandatory for usage of Loom
	StaticJsonDocument<1000> messageJson;	/// Document to read incoming data into

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Loom Subscribe Platform module constructor.
	///
	/// \param[in]	module_name			String | <"Internet-Plat"> | null | Subscribe platform module name
	LoomSubscribePlat( 
		const char*			module_name,
		LoomModule::Type	module_type,
		LoomModule::Type	internet_type
	);

	/// Destructor
	virtual ~LoomSubscribePlat() = default;

	/// Grab the internet platform specified by the ctor parameters
	void second_stage_ctor() override;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

    void Run() {}
	void		package(JsonObject json) override { /* do nothing for now */ }
	bool		dispatch(JsonObject json) override { /* do nothing for now */}

	virtual bool subscribe(JsonObject json) = 0;

	/// Version of log for use with LoomManager.
	/// Accesses Json from LoomManager
	bool		subscribe();


//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void print_config();
	virtual void print_state();

protected:


};

