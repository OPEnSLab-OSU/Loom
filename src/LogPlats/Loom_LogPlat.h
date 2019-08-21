#pragma once

#include "Loom_Module.h"


///////////////////////////////////////////////////////////////////////////////


/// Abstract base of logging platforms
class LoomLogPlat : public LoomModule
{

protected:

	bool			enable_rate_filter;		/// Whether or not to enable a minimum delay between logging
	uint16_t		min_filter_delay;		/// Minimum delay between logging (milliseconds) 	
	unsigned long	last_log_millis;		/// Value of millis() at last executed log time

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomLogPlat(	
			const char*			module_name,
			LoomModule::Type	module_type,
			bool				enable_rate_filter 		= true,
			uint16_t			min_filter_delay 		= 1000
	   );

	/// Destructor
	virtual ~LoomLogPlat() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	virtual bool	dispatch(JsonObject json) override {}
	virtual void 	package(JsonObject json) override {}

	/// Log a Json object
	/// \param[in] json		Json Object to log
	virtual void	log(JsonObject json) = 0;

	/// Version of log for use with LoomManager.
	/// Accesses Json from LoomManager
	void	log();		
	// virtual void	log();		

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config();

protected:

	/// Check against millis to determine if time since last
	/// log exceeds min time 
	/// \return True if enough time has elapsed
	bool			check_millis();

};


