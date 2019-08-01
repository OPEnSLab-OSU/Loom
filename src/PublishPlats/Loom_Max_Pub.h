#pragma once

#include "Loom_PublishPlat.h"


///////////////////////////////////////////////////////////////////////////////

// ### (LoomModule) | dependencies: [] | conflicts: []
/// Module taking in and translating JSON into data understood by the Google Sheets script API.
// ###
class Loom_MaxPub : public LoomPublishPlat 
{

protected:
	
	uint16_t	UDP_port;				/// Which UDP port to transmit on
	
	// LoomInternetPlat::UDPPtr UDP_Inst;	/// Pointer to UDP object

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	Loom_MaxPub(
		// uint8_t		internet_index	= 0,
		LoomModule::Type	internet_type,
		uint16_t			port	
	);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]  p     The array of constuctor args to expand
	Loom_MaxPub( JsonArrayConst p );

	// void second_stage_ctor() override;

	/// Destructor
	~Loom_MaxPub() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================


//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_state() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	uint16_t	get_port() { return UDP_port; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	void		set_port(uint16_t port);

protected:

	bool		send_to_internet(const JsonObject json, LoomInternetPlat* plat) override;

private:

};

