#pragma once

#include "PublishPlat.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomPublishPlat) | dependencies: [] | conflicts: []
/// Class description
// ###
class LoomPublishPlatTemplate : public LoomPublishPlat
{

protected:

	// Protected members here (are inherited)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomPublishPlatTemplate(
			LoomManager* manager,
const char* module_name			= "PublishPlatTemplate",
			uint			internet_index		= 1,
			int				ex_param			= 42
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	LoomPublishPlatTemplate(LoomManager* manager, JsonArrayConst p);
	
	/// Destructor
	virtual ~LoomPublishPlatTemplate() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void 		package(JsonObject json) override;
	bool		dispatch(JsonObject json) override;
	void		power_down() override {} 
	void		power_up() override {}

	bool		send_to_internet(const JsonObject json, LoomInternetPlat* plat) override;

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




