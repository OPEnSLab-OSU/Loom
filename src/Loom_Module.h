#pragma once
// #ifndef LOOM_MODULE_h
// #define LOOM_MODULE_h

#include "Loom_Manager.h"

#include "Loom_Misc.h"
#include "Loom_Translator.h"
#include "Loom_Macros.h"

#include <OSCBundle.h>
#include <ArduinoJson.h>

#include "Loom_Package.h"





class LoomManager; // Specify that LoomManager exists, defined in own file



/// Abstract root of Loom component modules inheritance hierarchy
class LoomModule
{

protected:

	ModuleType		module_type;

	/// Pointer to manager
	LoomManager*	device_manager;

	/// The name of the module (Should have a DEFAULT but can be overriden if provided to constructor)
	char			module_name[20];				
	
	/// Whether or not the module should be treated
	bool			active;
	/// Individually togglable debug statements
	bool			print_debug;				

	/// LPrint verbosity
	Verbosity		print_verbosity;		
	/// Package verbosity 
	Verbosity		package_verbosity;		

public:

	static char* enum_verbosity_string(Verbosity v);


	// config struct may go here if it is pointed to by a master flash struct
	// but can be protected if method loads the data from the FlashStorage struct


	// --- CONSTRUCTOR ---
	LoomModule();

	LoomModule( const char* module_name );// : LoomModule();

	LoomModule(JsonVariant settings);


	// LoomModule( char* module_name, LoomManager* LD );


// This version did not seem to work
	// LoomModule( char*      module_name 		 = "Unknown",
	// 			bool       active 			 = true,
	// 			bool       print_debug 		 = true,
	// 			Verbosity  print_verbosity   = VERB_LOW,
	// 			Verbosity  package_verbosity = VERB_LOW

	// 		  ) : LoomModule()
	// {
	// 	// LPrintln("LoomModule Constructor 1");
	// 	this->module_name 		= module_name;
	// 	this->active 			= active;
	// 	this->print_debug 		= print_debug;
	// 	this->print_verbosity 	= print_verbosity;
	// 	this->package_verbosity = package_verbosity;	
	// }


	// --- DESTRUCTOR ---
	virtual ~LoomModule();


	// --- PUBLIC METHODS ---


	ModuleType		get_module_type();

	/// Get the device manager class if linked
	/// \return Pointer to the LoomManager, Null if not linked
	LoomManager*	get_device_manager();

	/// Add pointer back to device manager.
	/// Generally only called when device manager links module
	/// to provide pointer both directions. 
	/// Derived modules may override this for increased function,
	/// such as linking a submanager or RTC module.
	/// \param[in]	LM	LoomManager to point to
	void	link_device_manager(LoomManager* LM);

	/// LPrint the module name as a label.
	/// Used for matching debug prints to corresponding module
	void			print_module_label();

	/// Display the configuration settings of the module
	virtual void	print_config();

	/// Display current state of the module
	virtual void	print_state();

	/// Take any relevant measurements
	virtual void	measure() = 0; // maybe drop to subclasses  (measure_aux in Manager wouldnt work then)

	// Append to a bundle 

	// Subclasses can provide defaults for id_prefix, permitting package(&bndl);
	/// Package a modules measurements or state.
	virtual void	package(OSCBundle& bndl, char* suffix="") = 0;

	virtual void 	package(JsonObject json) = 0;


	// Package but try to reference LoomManager for id_prefix
	// void package(OSCBundle& bndl);

	/// Message routing on an OSC message.
	// Only parsing the message should happen in message_route
	// Complete action should have its own method
	// Dispatch doesnt work unless the method is static, current use fullMatch instead
	virtual bool	message_route(OSCMessage& msg, int address_offset) = 0;

	// Not sure if there should be a verison that takes a bundle as well
		// Maybe in LoomManager , but not here
	// Might have a default value for address_offset to skip the /[D/S/F] section?
	// This might have be OSCMessage& msg instead

	// Modules might call parent message_route at the end of their own routing
		// Wont be called if a match is found
		// Would need to make sure the parent routing isn't called repeatedly
		// as different modules are iterated through


	/// Copy module name into buffer
	/// \param[out]	buf	The buffer to copy module name into
	void			get_module_name(char* buf);
	/// Get module name
	/// \return	Module name
	const char*		get_module_name();


	/// Set print verbosity 
	/// Controlls level of detail included in debug prints 
	void			set_print_verbosity(Verbosity v);
	/// Get print verbosity
	/// \return		The current verbosity setting
	Verbosity		get_print_verbosity();


	/// Set package verbosity.
	/// Controlls level of detail included in bundles
	/// \param[in]	v	The verbosity setting
	void			set_package_verbosity(Verbosity v);
	/// Get package verbosity
	/// \return		The current verbosity setting
	Verbosity		get_package_verbosity();


	/// Set whether or not the module should be treated as active
	/// \param[in]	enable	Whether or not to enable module
	void			set_active(bool enable);
	/// Get whether or not the module should be treated as active
	/// \return		Whether or not the module is active
	bool			get_active();


	/// Set whether or not debug print statements are enabled for this module
	/// \param[in]	enable	Whether or not to enable print statements
	void			set_print_debug(bool enable);
	/// Get whether or not debug prints are enabled
	/// \return		Whether or not print statements are enabled
	bool			get_print_debug();



	virtual void	power_down() {}
	virtual void	power_up() {}



// Maybe?
//  - not pure virtual because not everything will have a config (should default to empty function here)
	// Save a FlashStorage struct
	virtual void	save_config();
	// Load a FlashStorage struct, true if valid
	virtual bool	load_config();
	// Display config struct contents (as flash can be disabled this is a different method)
	virtual void	print_config_struct();



	// Optionally implementable interactive loop
	virtual void	REPL_loop();


protected:

	/// Determine what address should be used for an OSC bundle for this module
	/// \param[out]	address		The 
	/// \param[in]	
	void			resolve_bundle_address(char* address, char* suffix="");

private:


};


// #endif









