///////////////////////////////////////////////////////////////////////////////
///
/// @file		LogPlatTemplate.h
/// @brief		File for LogPlatTemplate definition
/// @author		FirstName LastName
/// @date		Year
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LogPlat.h"

namespace Loom {

///////////////////////////////////////////////////////////////////////////////


// ### (LogPlat) | dependencies: [] | conflicts: []
/// Class description
// ###
class LogPlatTemplate : public LogPlat
{

protected:

	// Protected members here (are inherited)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LogPlatTemplate(
			bool			enable_rate_filter		= true,
			uint			min_filter_delay		= 1000
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	LogPlatTemplate(JsonArrayConst p);

	/// Destructor
	virtual ~LogPlatTemplate() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void 		package(JsonObject json) override;
	bool		dispatch(JsonObject json) override;
	void		power_down() override {}
	void		power_up() override {}

	void		log(JsonObject json) override;

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
REGISTER(Module, LogPlatTemplate, "LogPlatTemplate");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom
