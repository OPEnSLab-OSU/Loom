///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Max_Sub.h
/// @brief		File for Loom_MaxSub definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "SubscribePlat.h"


#define UDP_RECEIVE_OFFSET 9000		///< UDP receiving port is this value + device instance number


///////////////////////////////////////////////////////////////////////////////
///
/// Module for receiving data from Max
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___max_sub.html)
/// - [Max 8](https://cycling74.com)
/// - [Loom Max Patches](https://github.com/OPEnSLab-OSU/Max-Loom2)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_MaxSub : public LoomSubscribePlat 
{

protected:
	
	uint16_t	UDP_port;				///< Which UDP port to receive on
	bool		auto_dispatch;			///< True to immediately call LoomManager::dispatch() when packet received

	LoomInternetPlat::UDPPtr UDP_Inst;	///< Pointer to UDP object

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]  internet_type		Set(LoomModule::Type) | <7001> | {7001("Ethernet"), 7002("WiFi")} | Code of the desired internet platform. 
	/// @param[in]	auto_dispatch		True to immediately call LoomManager::dispatch() when packet received
	Loom_MaxSub(
		LoomManager* manager,
		const LoomModule::Type	internet_type,
		const bool				auto_dispatch	
	); 

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]  p     The array of constuctor args to expand
	Loom_MaxSub(LoomManager* manager, JsonArrayConst p );

	void second_stage_ctor() override;

	/// Destructor
	~Loom_MaxSub() { if (UDP_Inst) UDP_Inst->stop(); }

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// No package necessary for subscribe platforms.
	/// Implement with empty body.
	void		package(JsonObject json) override { /* do nothing for now */ }	

	bool		subscribe(JsonObject json) override;

	using LoomSubscribePlat::subscribe;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the UDP port currently receiving on
	/// @return UDP port being used
	uint16_t	get_port() const { return UDP_port; }

	/// Get the last IP address received from
	/// @return Last IP address
	IPAddress	get_remote_IP() const { return (UDP_Inst) ? UDP_Inst->remoteIP() : IPAddress(0,0,0,0); }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set the UDP port to receive on.
	/// Changing the port from automatic port setting (based on instance number)
	/// may make the device stop receiving from Max MSP
	/// @param[in]	port	Port to set to
	void		set_port(const uint16_t port);

private:


};

