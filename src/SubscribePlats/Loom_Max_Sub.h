#pragma once

#include "Loom_SubscribePlat.h"


///////////////////////////////////////////////////////////////////////////////

// ### (LoomModule) | dependencies: [] | conflicts: []
/// Module taking in and translating JSON into data understood by the Google Sheets script API.
// ###
class Loom_MaxSub : public LoomSubscribePlat 
{

protected:
	
	
public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor.
	///
	Loom_MaxSub(); 

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]  p     The array of constuctor args to expand
	Loom_MaxSub( JsonArrayConst p );

	/// Destructor
	~Loom_MaxSub() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	bool	subscribe(const JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void	print_config() override;
	void	print_state() override;

private:


};

