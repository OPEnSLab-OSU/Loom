#pragma once

#include "Loom_Module.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomModule) | dependencies: [] | conflicts: []
/// Class description
// ###
class LoomModuleTemplate : public LoomModule
{

protected:

	// Protected members here (are inherited)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomModuleTemplate(
			const char*		module_name		= "ModuleTemplate",
			int 			ex_param		= 42
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	LoomModuleTemplate(JsonArrayConst p);
	
	/// Destructor
	virtual ~LoomModuleTemplate() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void 		package(JsonObject json) override;
	bool		cmd_route(JsonObject json) override;
	void		power_down() override {} 
	void		power_up() override {}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_state() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================


//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================


//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================



private:

	// Private members (are not inherited)

	// Auxiliary functions

};




