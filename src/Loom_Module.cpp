
#include "Loom_Module.h"

/////////////////////////////////////////////////////////////////////
char* LoomModule::enum_verbosity_string(Verbosity v)
{
	switch(v) {
		case Verbosity::V_OFF  : return "Off";
		case Verbosity::V_LOW  : return "Low";
		case Verbosity::V_HIGH : return "High";
	}
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
LoomModule::LoomModule() 
{
	// Println("LoomModule Default Constructor");
// 
	// this->module_name 	= "Unknown";
	// strcpy(this->module_name, "Unknown");

	snprintf(this->module_name, 20, "%s", "Unknown"); 
	
	active 				= true;
	print_debug 		= true;
	print_verbosity 	= Verbosity::V_LOW;
	package_verbosity 	= Verbosity::V_LOW;
	
	this->device_manager = nullptr;
}

/////////////////////////////////////////////////////////////////////
LoomModule::LoomModule( const char* module_name ) : LoomModule()
{
	// Println("LoomModule Constructor 1");

	// strcpy(this->module_name, module_name);

	// Println2("Module name:", module_name);
	snprintf(this->module_name, 20, "%s", module_name); 
// 
	// print_module_label();
	// Println("Begin Setup");
}

// /////////////////////////////////////////////////////////////////////
// // this version also links to parent device
// LoomModule::LoomModule( char* module_name, LoomManager* LD ) : LoomModule()
// {
// 	// Println("LoomModule Constructor 2");
// 	this->module_name   = module_name;
// 		// strcpy(this->module_name, module_name);

// 	// this->device_manager = LD;
// 	link_device_manager(LD);
// }

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
LoomModule::~LoomModule() {}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
LoomManager* LoomModule::get_device_manager()
{
	return device_manager;
}

/////////////////////////////////////////////////////////////////////
void LoomModule::link_device_manager(LoomManager* LM)
{
	if (LM == nullptr) return;
	device_manager = LM;
}

/////////////////////////////////////////////////////////////////////
void LoomModule::print_module_label()
{
	Print3("[", module_name, "] ");
}

/////////////////////////////////////////////////////////////////////
// Display configuration settings
void LoomModule::print_config()
{
	print_module_label();
	Println("Config:");
	Println3('\t', "Module Active       : ", (active)      ? "Enabled" : "Disabled" );
	Println3('\t', "Print Debug         : ", (print_debug) ? "Enabled" : "Disabled" );
	Println3('\t', "Print Verbosity     : ", enum_verbosity_string(print_verbosity) );
	Println3('\t', "Package Verbosity   : ", enum_verbosity_string(package_verbosity) );
}

/////////////////////////////////////////////////////////////////////
// Display current state
void LoomModule::print_state() 
{

}

/////////////////////////////////////////////////////////////////////
// void LoomModule::package(OSCBundle* bndl)
// {
// 	char buf[20];
// 	resolve_bundle_address(buf);
// 	package(bndl, buf);
// }

/////////////////////////////////////////////////////////////////////
// void LoomModule::resolve_bundle_prefix(char* prefix) 
// {
// 	if (device_manager != nullptr) {
// 		device_manager->packet_header_device(prefix);
// 	} else {
// 		strcpy(prefix, module_name);
// 	}
// }

/////////////////////////////////////////////////////////////////////
void LoomModule::resolve_bundle_address(char* address, char* suffix)
{
	if (device_manager != nullptr) {
		char tmp[30];
		device_manager->packet_header_device(tmp);
		sprintf(address, "%s/%s", tmp, module_name);
		// device_manager->packet_header_device(address);

	} else {
		// strcpy(address, module_name);
		sprintf(address, "/%s", module_name);
	}

	if (strlen(suffix) > 0) {
		sprintf(address, "%s/%s", address, suffix);
	} 
}




	// void LoomModule::measure() = 0;

	// Append to a bundle 
	// void LoomModule::package(OSCBundle* bndl, char* id_prefix) = 0;



	// Perform message routing on an OSC message
	// Only parsing the message should happen in message_route
	// Complete action should have its own method
	// bool LoomModule::message_route(OSCMessage& msg, int address_offset) = 0;

	// Not sure if there should be a verison that takes a bundle as well
		// Maybe in LoomManager , but not here
	// Might have a default value for address_offset to skip the /[D/S/F] section?
	// This might have be OSCMessage& msg instead

	// Modules might call parent message_route at the end of their own routing
		// Wont be called if a match is found
		// Would need to make sure the parent routing isn't called repeatedly
		// as different modules are iterated through

/////////////////////////////////////////////////////////////////////
// Copy module name into buffer
void LoomModule::get_module_name(char* buf) 
{ 
	strcpy(buf, module_name); 
}

/////////////////////////////////////////////////////////////////////
// Return module name char*
const char* LoomModule::get_module_name() 
{ 
	return module_name; 
}

/////////////////////////////////////////////////////////////////////
// Set print verbosity 
void LoomModule::set_print_verbosity(Verbosity v) 
{ 
	print_verbosity = v; 
}

/////////////////////////////////////////////////////////////////////
// Get print verbosity
Verbosity LoomModule::get_print_verbosity() 
{ 
	return print_verbosity; 
}

/////////////////////////////////////////////////////////////////////
// Set package verbosity (what gets included in bundle)
void LoomModule::set_package_verbosity(Verbosity v) 
{ 
	package_verbosity = v; 
}

/////////////////////////////////////////////////////////////////////
// Get package verbosity
Verbosity LoomModule::get_package_verbosity() 
{ 
	return package_verbosity; 
}

/////////////////////////////////////////////////////////////////////
// Set whether or not to be active
void LoomModule::set_active(bool enable) 
{ 
	active = enable; 
}

/////////////////////////////////////////////////////////////////////
// Get whether or not device is active
bool LoomModule::get_active() 
{ 
	return active; 
}

/////////////////////////////////////////////////////////////////////
// Set whether or not to use debug print statements
void LoomModule::set_print_debug(bool enable) 
{ 
	print_debug = enable; 
}

/////////////////////////////////////////////////////////////////////
// Get whether or not debug prints are enabled
bool LoomModule::get_print_debug() 
{ 
	return print_debug; 
}

/////////////////////////////////////////////////////////////////////
// Maybe?
//  - not pure because not everything will have a config (should default to empty function here)
// Save a FlashStorage struct
void LoomModule::save_config() {}

/////////////////////////////////////////////////////////////////////
// Load a FlashStorage struct, true if valid
bool LoomModule::load_config() {}

/////////////////////////////////////////////////////////////////////
// Display config struct contents (as flash can be disabled this is a different method)
void LoomModule::print_config_struct() {}


/////////////////////////////////////////////////////////////////////
// Optionally implementable interactive loop
void LoomModule::REPL_loop() {}


