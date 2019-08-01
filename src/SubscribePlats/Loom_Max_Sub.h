#pragma once

#include "Loom_SubscribePlat.h"


///////////////////////////////////////////////////////////////////////////////

// ### (LoomModule) | dependencies: [] | conflicts: []
/// Module taking in and translating JSON into data understood by the Google Sheets script API.
// ###
class Loom_MaxSub : public LoomSubscribePlat 
{

protected:
	
	uint16_t	UDP_port;				/// Which UDP port to receive on

	LoomInternetPlat::UDPPtr UDP_Inst;	/// Pointer to UDP object

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor.
	///
	Loom_MaxSub(
		LoomModule::Type	internet_type,
		uint16_t			port	
	); 

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]  p     The array of constuctor args to expand
	Loom_MaxSub( JsonArrayConst p );

	void second_stage_ctor() override;

	/// Destructor
	~Loom_MaxSub() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	bool	subscribe(JsonObject json) override;

	using LoomSubscribePlat::subscribe;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void	print_config() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	uint16_t	get_port() { return UDP_port; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	void		set_port(uint16_t port);

private:


};

