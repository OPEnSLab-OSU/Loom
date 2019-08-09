#pragma once

#include "Loom_PublishPlat.h"
#include "Loom_Manager.h"


#define UDP_SEND_OFFSET 8000


///////////////////////////////////////////////////////////////////////////////

// ### (LoomModule) | dependencies: [] | conflicts: []
/// Module taking in and translating JSON into data understood by the Google Sheets script API.
// ###
class Loom_MaxPub : public LoomPublishPlat 
{

protected:
	
	uint16_t	UDP_port;				/// Which UDP port to transmit on
	IPAddress	remoteIP;				/// Which IP address to send to


	LoomInternetPlat::UDPPtr UDP_Inst;	/// Pointer to UDP object

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	Loom_MaxPub(
		LoomModule::Type	internet_type
	);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]  p     The array of constuctor args to expand
	Loom_MaxPub( JsonArrayConst p );
// 
	void second_stage_ctor() override;

	/// Destructor
	~Loom_MaxPub() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	bool		dispatch(JsonObject json) override;

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

	/// Set the UDP port to send on
	/// \param[in]	port	The UDP port to send on
	void		set_port(uint16_t port);

	/// Set the IP address to send to
	/// \param[in]	ip	The IP address to set to
	void		set_ip(IPAddress ip);

	/// Set the IP addres to send to by getting remote IP from MaxSub if available
	void		set_ip();

protected:

	bool		send_to_internet(const JsonObject json, LoomInternetPlat* plat) override;

private:

};

