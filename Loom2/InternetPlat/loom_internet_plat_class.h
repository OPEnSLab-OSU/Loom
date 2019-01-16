
#ifndef LOOM_INTERNET_PLAT_h
#define LOOM_INTERNET_PLAT_h


#include "loom_abstract_module_class.h"

#include <OSCBundle.h>


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

	void print_config();


	// Should this be external?
	bool log_to_pushingbox(OSCBundle* bndl);

private:

// --- PRIVATE MEMBERS ---

// --- PRIVATE METHODS --- 



};



#endif

