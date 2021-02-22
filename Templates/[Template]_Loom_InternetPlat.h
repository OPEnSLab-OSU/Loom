#pragma once

#include "InternetPlat.h"


///////////////////////////////////////////////////////////////////////////////


// ### (InternetPlat) | dependencies: [] | conflicts: []
/// Class description
// ###
class InternetPlatTemplate : public InternetPlat
{

protected:

	// Protected members here (are inherited)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	InternetPlatTemplate(
			LoomManager* manager,
const char* module_name			= "InternetPlatTemplate",
			int				ex_param			= 42
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	InternetPlatTemplate(LoomManager* manager, JsonArrayConst p);
	
	/// Destructor
	virtual ~InternetPlatTemplate() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void 		package(JsonObject json) override;
	bool		dispatch(JsonObject json) override;
	void		power_down() override {} 
	void		power_up() override {}

	void 		connect() override;
	bool 		is_connected() override;
	uint32_t 	get_time() override;
	Client& 	http_request(const char* domain, const char* url, const char* body, const char* verb) override;

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




