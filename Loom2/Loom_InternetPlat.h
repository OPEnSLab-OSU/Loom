
#ifndef LOOM_INTERNET_PLAT_h
#define LOOM_INTERNET_PLAT_h

#include "Loom_Module.h"


enum InternetPlatform { INT_WIFI, ETHERNET, CELLULAR_2G };



class LoomInternetPlat : public LoomModule
{

protected:
	// --- PROTECTED MEMBERS ---
	
	
public:

	// --- PUBLIC MEMBERS ---


	// --- CONSTRUCTOR ---
	LoomInternetPlat(	char* module_name

					);

	// --- DESTRUCTOR ---
	~LoomInternetPlat();

	static char* enum_internet_plat_string(InternetPlatform p);


	// --- PUBLIC METHODS ---

	virtual void print_config();
	virtual void print_state();

	virtual void measure() {}
	virtual void package(OSCBundle* bndl) {}
	virtual bool message_route(OSCMessage* msg, int address_offset) {}

	virtual bool connect() = 0;
	virtual bool is_connected() = 0;
	virtual uint32_t get_time() = 0;



	// Should this be external?
	virtual bool log_to_pushingbox(OSCBundle* bndl);

private:

// --- PRIVATE MEMBERS ---

// --- PRIVATE METHODS --- 



};



#endif

