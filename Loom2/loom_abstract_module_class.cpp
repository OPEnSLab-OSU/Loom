
#include "loom_abstract_module_class.h"
#include "loom_device_class.h"


#include "loom_macros.h"



// char* enum_verbosity_string(Verbosity v)
// {
// 	switch(v) {
// 		case VERB_OFF  : return "Off";
// 		case VERB_LOW  : return "Low";
// 		case VERB_HIGH : return "High";
// 	}
// }

char* LoomModule::enum_verbosity_string(Verbosity v)
{
	switch(v) {
		case VERB_OFF  : return "Off";
		case VERB_LOW  : return "Low";
		case VERB_HIGH : return "High";
	}
}




// --- CONSTRUCTOR ---
LoomModule::LoomModule() 
{
	// LOOM_DEBUG_Println("LoomModule Default Constructor");

	this->module_name 	= "Unknown";
	active 				= true;
	print_debug 		= true;
	print_verbosity 	= VERB_LOW;
	package_verbosity 	= VERB_LOW;
	
	this->parent_device = NULL;
}


LoomModule::LoomModule( char* module_name ) : LoomModule()
{
	// LOOM_DEBUG_Println("LoomModule Constructor 1");
	this->module_name = module_name;
}

// this version also links to parent device
LoomModule::LoomModule( char* module_name, LoomDevice* LD ) : LoomModule()
{
	// LOOM_DEBUG_Println("LoomModule Constructor 2");
	this->module_name   = module_name;
	// this->parent_device = LD;
	link_parent_device(LD);
}

// This version did not seem to work
	// LoomModule( char*      module_name 		 = "Unknown",
	// 			bool       active 			 = true,
	// 			bool       print_debug 		 = true,
	// 			Verbosity  print_verbosity   = VERB_LOW,
	// 			Verbosity  package_verbosity = VERB_LOW

	// 		  ) : LoomModule()
	// {
	// 	// LOOM_DEBUG_Println("LoomModule Constructor 1");
	// 	this->module_name 		= module_name;
	// 	this->active 			= active;
	// 	this->print_debug 		= print_debug;
	// 	this->print_verbosity 	= print_verbosity;
	// 	this->package_verbosity = package_verbosity;	
	// }


// --- DESTRUCTOR ---
LoomModule::~LoomModule() {}


// --- PUBLIC METHODS ---


LoomDevice* LoomModule::get_parent_device()
{
	return parent_device;
}


void LoomModule::link_parent_device(LoomDevice* LD)
{
	if (LD == NULL) return;

	parent_device = LD;

	char buf[50];
	LD->get_device_name(buf);

	print_module_label();
	LOOM_DEBUG_Println2("Linked as module of ", buf);
}


void LoomModule::print_module_label()
{
	LOOM_DEBUG_Print3("[", module_name, "] ");
}


// Display configuration settings
void LoomModule::print_config()
{
	LOOM_DEBUG_Println2(module_name, " Config:");
	LOOM_DEBUG_Println3('\t', "Module Active       : ", (active)      ? "Enabled" : "Disabled" );
	LOOM_DEBUG_Println3('\t', "Print Debug         : ", (print_debug) ? "Enabled" : "Disabled" );
	LOOM_DEBUG_Println3('\t', "Print Verbosity     : ", enum_verbosity_string(print_verbosity) );
	LOOM_DEBUG_Println3('\t', "Package Verbosity   : ", enum_verbosity_string(package_verbosity) );
}

// Display current state
void LoomModule::print_state() 
{

}


	// void LoomModule::measure() = 0;

	// Append to a bundle 
	// void LoomModule::package(OSCBundle* bndl, char* id_prefix) = 0;



	// Perform message routing on an OSC message
	// Only parsing the message should happen in message_route
	// Complete action should have its own method
	// bool LoomModule::message_route(OSCMessage* msg, int address_offset) = 0;

	// Not sure if there should be a verison that takes a bundle as well
		// Maybe in LoomDevice , but not here
	// Might have a default value for address_offset to skip the /[D/S/F] section?
	// This might have be OSCMessage& msg instead

	// Modules might call parent message_route at the end of their own routing
		// Wont be called if a match is found
		// Would need to make sure the parent routing isn't called repeatedly
		// as different modules are iterated through


// Copy module name into buffer
void LoomModule::get_module_name(char* buf) 
{ 
	strcpy(buf, module_name); 
}


// Return module name char*
char* LoomModule::get_module_name() 
{ 
	return module_name; 
}


// Set print verbosity 
void LoomModule::set_print_verbosity(Verbosity v) 
{ 
	print_verbosity = v; 
}


// Get print verbosity
Verbosity LoomModule::get_print_verbosity() 
{ 
	return print_verbosity; 
}


// Set package verbosity (what gets included in bundle)
void LoomModule::set_package_verbosity(Verbosity v) 
{ 
	package_verbosity = v; 
}


// Get package verbosity
Verbosity LoomModule::get_package_verbosity() 
{ 
	return package_verbosity; 
}


// Set whether or not to be active
void LoomModule::set_active(bool enable) 
{ 
	active = enable; 
}


// Get whether or not device is active
bool LoomModule::get_active() 
{ 
	return active; 
}


// Set whether or not to use debug print statements
void LoomModule::set_print_debug(bool enable) 
{ 
	print_debug = enable; 
}


// Get whether or not debug prints are enabled
bool LoomModule::get_print_debug() 
{ 
	return print_debug; 
}


// Maybe?
//  - not pure because not everything will have a config (should default to empty function here)
// Save a FlashStorage struct
void LoomModule::save_config() {}
// Load a FlashStorage struct, true if valid
bool LoomModule::load_config() {}
// Display config struct contents (as flash can be disabled this is a different method)
void LoomModule::print_config_struct() {}



// Optionally implementable interactive loop
void LoomModule::REPL_loop() {}


