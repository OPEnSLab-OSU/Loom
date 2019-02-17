
#ifndef LOOM_LOG_PLAT_h
#define LOOM_LOG_PLAT_h

#include "Loom_Module.h"

#include <OSCBundle.h>



enum LogPlatform { SDCARD, PUSHINGBOX, OLED, SERIAL_MON };



class LoomLogPlat : public LoomModule
{

protected:
	// --- PROTECTED MEMBERS ---
	bool          enable_rate_filter; 	// Whether or not to enable a minimum delay between logging
	uint          min_filter_delay;		// Minimum delay between logging (milliseconds)
	unsigned long last_log_millis;		// Value of millis() at last executed log

public:

	// --- PUBLIC MEMBERS ---

	static char* enum_log_plat_string(LogPlatform p);


	// --- CONSTRUCTOR ---
	LoomLogPlat(	char* module_name,

					bool enable_rate_filter 	= true,
					uint min_filter_delay 		= 1000

			   );


	// --- DESTRUCTOR ---
	virtual ~LoomLogPlat();


	// --- PUBLIC METHODS ---

	virtual void print_config();
	virtual void package(OSCBundle& bndl, char* suffix="");
	virtual void measure();
	virtual bool message_route(OSCMessage* msg, int address_offset);
	virtual void log_bundle(OSCBundle* bndl) = 0;

private:

protected:

	bool check_millis();

};



#endif
