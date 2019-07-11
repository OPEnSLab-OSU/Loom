#pragma once

#include "Loom_Module.h"


///////////////////////////////////////////////////////////////////////////////


/// Abstract base of logging platforms
class LoomLogPlat : public LoomModule
{

public:

	/// Differnt Logging platforms supported
	enum class Platform { 
		SDCARD, 		///< Serial Monitor
		PUSHINGBOX, 	///< PushingBox 
		OLED, 			///< OLED Display
		SERIAL_MON 		///< Serial Monitor
	};

protected:

	bool			enable_rate_filter;		/// Whether or not to enable a minimum delay between logging
	uint			min_filter_delay;		/// Minimum delay between logging (milliseconds) 	
	unsigned long	last_log_millis;		/// Value of millis() at last executed log time

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomLogPlat(	
			const char*		module_name,

			bool			enable_rate_filter 		= true,
			uint			min_filter_delay 		= 1000
	   );

	/// Destructor
	virtual ~LoomLogPlat() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	virtual bool	cmd_route(JsonObject) override {}
	virtual void 	package(JsonObject json) override {}

	/// Log a Json object
	/// \param[in] json		Json Object to log
	virtual void	log(JsonObject json) = 0;

	/// Version of log for use with LoomManager.
	/// Accesses Json from LoomManager
	void			log();

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config();

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	/// Get c-string of name associated with log platform enum
	/// \return c-string of log platform
	static const char* enum_log_plat_string(Platform p);

protected:

	/// Check against millis to determine if time since last
	/// log exceeds min time 
	/// \return True if enough time has elapsed
	bool			check_millis();

};


