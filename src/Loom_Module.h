#pragma once

#include "Loom_Manager.h"
#include "Loom_Misc.h"
#include "Loom_Translator.h"
#include "Loom_Macros.h"
#include "Loom_Package.h"

#include "ArduinoJson.h"



class LoomManager; // Specify that LoomManager exists, defined in own file



/// Abstract root of Loom component modules inheritance hierarchy
class LoomModule
{

protected:

	ModuleType		module_type;		/// Module type (hub / node)
	LoomManager*	device_manager;		/// Pointer to manager
	char			module_name[20];	/// The name of the module (Should have a DEFAULT but can be overriden if provided to constructor)
	bool			active;				/// Whether or not the module should be treated as active
	bool			print_debug;		/// Individually togglable debug statements
	Verbosity		print_verbosity;	/// Print verbosity
	Verbosity		package_verbosity;	/// Package verbosity 

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomModule();

	/// Constructor
	LoomModule( const char* module_name );

	/// Destructor
	virtual ~LoomModule() = default;

	/// Second stage construction.
	/// Perform any actions that need all of the devices to be
	/// initized beforehand - such as looking up an internet platform
	virtual void	second_stage_ctor();

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	// /// Take any relevant measurements
	// virtual void	measure() = 0; // maybe drop to subclasses  (measure_aux in Manager wouldnt work then)

	/// Package a modules measurements or state.
	/// \param[out]	json	Object to put data into
	virtual void 	package(JsonObject json) = 0;

	/// Route command to driver 
	virtual bool	cmd_route(JsonObject) = 0;

	/// Turn off any hardware
	virtual void	power_down() {}

	/// Turn on any hardware
	virtual void	power_up() {}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	/// Display the configuration settings of the module
	virtual void	print_config();

	/// Display current state of the module
	virtual void	print_state();

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get module type
	/// \return Module type
	ModuleType		get_module_type();

	/// Get the device manager class if linked
	/// \return Pointer to the LoomManager, Null if not linked
	LoomManager*	get_device_manager();

	/// Copy module name into buffer
	/// \param[out]	buf	The buffer to copy module name into
	void			get_module_name(char* buf);
	/// Get module name
	/// \return	Module name
	const char*		get_module_name();

	/// Get print verbosity
	/// \return		The current verbosity setting
	Verbosity		get_print_verbosity();

	/// Get package verbosity
	/// \return		The current verbosity setting
	Verbosity		get_package_verbosity();

	/// Get whether or not the module should be treated as active
	/// \return		Whether or not the module is active
	bool			get_active();

	/// Get whether or not debug prints are enabled
	/// \return		Whether or not print statements are enabled
	bool			get_print_debug();

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Add pointer back to device manager.
	/// Generally only called when device manager links module
	/// to provide pointer both directions. 
	/// Derived modules may override this for increased function,
	/// such as linking a submanager or RTC module.
	/// \param[in]	LM	LoomManager to point to
	virtual void	link_device_manager(LoomManager* LM);

	/// Set print verbosity 
	/// Controlls level of detail included in debug prints 
	void			set_print_verbosity(Verbosity v);

	/// Set package verbosity.
	/// Controlls level of detail included in bundles
	/// \param[in]	v	The verbosity setting
	void			set_package_verbosity(Verbosity v);

	/// Set whether or not the module should be treated as active
	/// \param[in]	enable	Whether or not to enable module
	void			set_active(bool enable);

	/// Set whether or not debug print statements are enabled for this module
	/// \param[in]	enable	Whether or not to enable print statements
	void			set_print_debug(bool enable);

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	/// Get string of name associated with verbosity enum
	/// \return String of verbosity
	static char*	enum_verbosity_string(Verbosity v);



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

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	/// LPrint the module name as a label.
	/// Used for matching debug prints to corresponding module
	void			print_module_label();

private:


};




