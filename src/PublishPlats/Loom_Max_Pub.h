#pragma once

#include "Loom_PublishPlat.h"


///////////////////////////////////////////////////////////////////////////////

// ### (LoomModule) | dependencies: [] | conflicts: []
/// Module taking in and translating JSON into data understood by the Google Sheets script API.
// ###
class Loom_MaxPub : public LoomPublishPlat 
{

protected:
	
	
public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	Loom_MaxPub();

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]  p     The array of constuctor args to expand
	Loom_MaxPub( JsonArrayConst p );

	/// Destructor
	~Loom_MaxPub() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	bool		send_to_internet(const JsonObject json, LoomInternetPlat* plat) override {}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_state() override;

private:

};

