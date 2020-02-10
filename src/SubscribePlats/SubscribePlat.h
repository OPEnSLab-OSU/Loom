///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_SubscribePlat.h
/// @brief		File for LoomSubscribePlat definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Module.h"
#include "../InternetPlats/InternetPlat.h"


///////////////////////////////////////////////////////////////////////////////
///
/// Abstract subscribe platform module.
///
/// All SubscribePlat modules inherit from this class.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_subscribe_plat.html)
///
///////////////////////////////////////////////////////////////////////////////
class LoomSubscribePlat : public LoomModule
{

protected:

	/// Pointer to internet platform to use to publish
	LoomInternetPlat*		m_internet;
	
	/// Type of internet platform used to publish.
	/// Needed because finding the module for m_internet happens in second_stage_ctor(),
	/// rather than the regular constructor.
	const LoomModule::Type	internet_type;

	/// Subscribe Platforms need their own JsonDocument because an incoming message
	/// can only be deserialized into JsonDocuments, not JsonObjects.
	/// And it seemed bad design to pass around references to the LoomManager's
	/// internal JsonDocument. 
	/// Also as the LoomManager is intended to be non-mandatory for usage of Loom
	StaticJsonDocument<1000> messageJson;	/// Document to read incoming data into

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor.
	///
	/// @param[in]	module_name			String | <"Internet-Plat"> | null | Publish platform module name
	/// @param[in]	module_type			Type of the module (provided by derived classes)
	/// @param[in]  internet_type		Set(LoomModule::Type) | <7001> | {7001("Ethernet"), 7002("WiFi")} | Code of the desired internet platform. 
	LoomSubscribePlat(
		LoomManager* manager,
		const char*				module_name,
		const LoomModule::Type	module_type,
		const LoomModule::Type	internet_type
	);

	/// Destructor
	virtual ~LoomSubscribePlat() = default;

	/// Grab the internet platform specified by the ctor parameters
	void second_stage_ctor() override;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// Request / try to receive data from platform subscribed to 
	/// @param[out]	json	Received data 
	/// @return True if received anything
	virtual bool	subscribe(JsonObject json) = 0;

	/// Version of log for use with LoomManager.
	/// Accesses Json from LoomManager
	/// @return True if received anything
	bool			subscribe();

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() const override;
	virtual void	print_state() const override;

protected:


};

