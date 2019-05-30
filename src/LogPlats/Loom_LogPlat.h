
#pragma once

#include "Loom_Module.h"


/// Differnt Logging platforms supported
enum class LogPlatform { 
	SDCARD, 		///< Serial Monitor
	PUSHINGBOX, 	///< PushingBox 
	OLED, 			///< OLED Display
	SERIAL_MON 		///< Serial Monitor
};


/// Abstract base of logging platforms
class LoomLogPlat : public LoomModule
{

protected:

	/// Whether or not to enable a minimum delay between logging
	bool			enable_rate_filter;

	/// Minimum delay between logging (milliseconds) 	
	uint			min_filter_delay;

	/// Value of millis() at last executed log time
	unsigned long	last_log_millis;		

public:

	static char* enum_log_plat_string(LogPlatform p);


	// --- CONSTRUCTOR ---
	LoomLogPlat(	
			const char*		module_name,

			bool			enable_rate_filter 		= true,
			uint			min_filter_delay 		= 1000
	   );


	// --- DESTRUCTOR ---
	virtual ~LoomLogPlat();


	// --- PUBLIC METHODS ---

	// Inherited Methods
	virtual void	print_config();
	virtual void	measure();
	// virtual bool	message_route(OSCMessage& msg, int address_offset);
	virtual bool	cmd_route(JsonObject) {}
	virtual void	log(JsonObject json) = 0;
	virtual void 	package(JsonObject json) {}

private:

protected:

	bool			check_millis();

};


