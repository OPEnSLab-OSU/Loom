///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_LogPlat.h
/// @brief		File for LoomLogPlat definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Module.h"


///////////////////////////////////////////////////////////////////////////////
///
/// Abstract base of logging platforms.
///
/// All logging modules inherit from this class.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_log_plat.html)
///
///////////////////////////////////////////////////////////////////////////////
class LoomLogPlat : public LoomModule
{

protected:

	bool			enable_rate_filter;		///< Whether or not to enable a minimum delay between logging
	uint16_t		min_filter_delay;		///< Minimum delay between logging (milliseconds) 	
	unsigned long	last_log_millis;		///< Value of millis() at last executed log time

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// @param[in]	module_name			Name of the module (provided by derived classes)
	/// @param[in] 	module_type			Type of the module (provided by derived classes)
	/// @param[in] 	enable_rate_filter	Whether or not to impose maximum update rate
	/// @param[in] 	min_filter_delay	Minimum update delay, if enable_rate_filter enabled
	LoomLogPlat(	
			LoomManager* manager,
			const char* module_name,
			const LoomModule::Type	module_type,
			const bool				enable_rate_filter	= true,
			const uint16_t			min_filter_delay	= 1000
		);

	/// Destructor
	virtual ~LoomLogPlat() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// No package necessary for logging platforms.
	/// implement with empty body.
	virtual void 	package(JsonObject json) override {}

	/// Log a Json object
	/// @param[in] json		Json Object to log
	virtual bool	log(JsonObject json) = 0;

	/// Version of log for use with LoomManager.
	/// Accesses Json from LoomManager.
	/// Calls derived classes implementations of log(JsonObject json)
	bool			log();		

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() const override;

protected:

	/// Check against millis to determine if time since last
	/// log exceeds min time 
	/// @return True if enough time has elapsed
	bool			check_millis();

};


