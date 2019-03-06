
#ifndef LOOM_MODULE_h
#define LOOM_MODULE_h

#include "Loom_Manager.h"

#include "Loom_Misc.h"
#include "Loom_Translator.h"
#include "Loom_Macros.h"

#include <OSCBundle.h>


// Maybe move out to global so that LoomManager can reference this as well
// enum Verbosity { VERB_OFF, VERB_LOW, VERB_HIGH }; // Move back to "loom_abstract_module_class.h"


class LoomManager; // Specify that LoomManager exists, defined in own file




class LoomModule
{

protected:

	LoomManager* device_manager; // rename to manager

	char* module_name;				// The name of the module    		Should have a DEFAULT but can be overriden if provided to constructor
	
	bool  active;					// Whether or not the device is currently active (not sure if this will be a setting)
	bool  print_debug;				// Individually togglable debug statements

	Verbosity print_verbosity;		// Print verbosity
	Verbosity package_verbosity;	// Package verbosity 	


	// int blink_sequence[] ?

public:



	static char* enum_verbosity_string(Verbosity v);


	// config struct may go here if it is pointed to by a master flash struct
	// but can be protected if method loads the data from the FlashStorage struct


	// --- CONSTRUCTOR ---
	LoomModule();

	LoomModule( char* module_name );// : LoomModule();

	LoomModule( char* module_name, LoomManager* LD );


// This version did not seem to work
	// LoomModule( char*      module_name 		 = "Unknown",
	// 			bool       active 			 = true,
	// 			bool       print_debug 		 = true,
	// 			Verbosity  print_verbosity   = VERB_LOW,
	// 			Verbosity  package_verbosity = VERB_LOW

	// 		  ) : LoomModule()
	// {
	// 	// Println("LoomModule Constructor 1");
	// 	this->module_name 		= module_name;
	// 	this->active 			= active;
	// 	this->print_debug 		= print_debug;
	// 	this->print_verbosity 	= print_verbosity;
	// 	this->package_verbosity = package_verbosity;	
	// }


	// --- DESTRUCTOR ---
	~LoomModule();


	// --- PUBLIC METHODS ---

	LoomManager* get_device_manager();

	void link_device_manager(LoomManager* LD);

	void print_module_label();

	// Display configuration settings
	virtual void print_config();

	// Display current state
	virtual void print_state();


	virtual void measure() = 0; // maybe drop to subclasses  (measure_aux in Manager wouldnt work then)

	// Append to a bundle 

	// Subclasses can provide defaults for id_prefix, permitting package(&bndl);
	virtual void package(OSCBundle& bndl, char* suffix="") = 0;

	// Package but try to reference LoomManager for id_prefix
	// void package(OSCBundle& bndl);

	// Perform message routing on an OSC message
	// Only parsing the message should happen in message_route
	// Complete action should have its own method
	// Dispatch doesnt work unless the method is static, current use fullMatch instead
	virtual bool message_route(OSCMessage& msg, int address_offset) = 0;

	// Not sure if there should be a verison that takes a bundle as well
		// Maybe in LoomManager , but not here
	// Might have a default value for address_offset to skip the /[D/S/F] section?
	// This might have be OSCMessage& msg instead

	// Modules might call parent message_route at the end of their own routing
		// Wont be called if a match is found
		// Would need to make sure the parent routing isn't called repeatedly
		// as different modules are iterated through


	// Copy module name into buffer
	void get_module_name(char* buf);
	// Return module name char*
	char* get_module_name();



	// Set print verbosity 
	void set_print_verbosity(Verbosity v);
	// Get print verbosity
	Verbosity get_print_verbosity();


	// Set package verbosity (what gets included in bundle)
	void set_package_verbosity(Verbosity v);
	// Get package verbosity
	Verbosity get_package_verbosity();


	// Set whether or not to be active
	void set_active(bool enable);
	// Get whether or not device is active
	bool get_active();


	// Set whether or not to use debug print statements
	void set_print_debug(bool enable);
	// Get whether or not debug prints are enabled
	bool get_print_debug();



// Maybe?
//  - not pure virtual because not everything will have a config (should default to empty function here)
	// Save a FlashStorage struct
	virtual void save_config();
	// Load a FlashStorage struct, true if valid
	virtual bool load_config();
	// Display config struct contents (as flash can be disabled this is a different method)
	virtual void print_config_struct();



	// Optionally implementable interactive loop
	virtual void REPL_loop();


protected:

	// void resolve_bundle_address(char* prefix);  // remove

	void resolve_bundle_address(char* address, char* suffix="");


private:


};


#endif









