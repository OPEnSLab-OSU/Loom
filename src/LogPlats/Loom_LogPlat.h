#pragma once

#include "Loom_Module.h"


///////////////////////////////////////////////////////////////////////////////


/// Abstract base of logging platforms
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
	/// \param[in]	module_name			Name of the module (provided by derived classes)
	/// \param[in] 	module_type			Type of the module (provided by derived classes)
	/// \param[in] 	enable_rate_filter	Whether or not to impose maximum update rate
	/// \param[in] 	min_filter_delay	Minimum update delay, if enable_rate_filter enabled
	LoomLogPlat(	
			const char*			module_name,
			LoomModule::Type	module_type,
			bool				enable_rate_filter 	= true,
			uint16_t			min_filter_delay 	= 1000
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
	/// \param[in] json		Json Object to log
	virtual void	log(JsonObject json) = 0;

	/// Version of log for use with LoomManager.
	/// Accesses Json from LoomManager.
	/// Calls derived classes implementations of log(JsonObject json)
	void			log();		

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() override;

protected:

	/// Check against millis to determine if time since last
	/// log exceeds min time 
	/// \return True if enough time has elapsed
	bool			check_millis();

};


