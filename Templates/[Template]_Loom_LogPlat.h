#pragma once

#include "LogPlat.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomLogPlat) | dependencies: [] | conflicts: []
/// Class description
// ###
class LoomLogPlatTemplate : public LoomLogPlat
{

protected:

	// Protected members here (are inherited)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomLogPlatTemplate(
			LoomManager* manager,
const char* module_name				= "LogPlatTemplate",
			bool			enable_rate_filter		= true,
			uint			min_filter_delay		= 1000		
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	LoomLogPlatTemplate(LoomManager* manager, JsonArrayConst p);
	
	/// Destructor
	virtual ~LoomLogPlatTemplate() = default;

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




