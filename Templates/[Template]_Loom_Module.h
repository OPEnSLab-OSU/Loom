///////////////////////////////////////////////////////////////////////////////
///
/// @file		ModuleTemplate.h
/// @brief		File for ModuleTemplate definition
/// @author		FirstName LastName
/// @date		Year
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Module.h"

namespace Loom {

///////////////////////////////////////////////////////////////////////////////


// ### (Module) | dependencies: [] | conflicts: []
/// Class description
// ###
class ModuleTemplate : public Module
{

protected:

	// Protected members here (are inherited)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	ModuleTemplate(
			int 			ex_param		= 42
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	ModuleTemplate(JsonArrayConst p);

	/// Destructor
	virtual ~ModuleTemplate() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void 		package(JsonObject json) override;
	bool		dispatch(JsonObject json) override;
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

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, ModuleTemplate, "ModuleTemplate");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom
