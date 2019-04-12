
#include "Loom_InternetPlat.h"

/////////////////////////////////////////////////////////////////////
char* LoomInternetPlat::enum_internet_plat_string(InternetPlatform p)
{
	switch(p) {
		case INT_WIFI			: return "WiFi";
		case INT_ETHERNET		: return "Ethernet";
		case INT_CELLULAR_2G	: return "Cellular";
	}
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
LoomInternetPlat::LoomInternetPlat(	
		char* module_name
	) 
	: LoomModule( module_name )
{

}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
LoomInternetPlat::~LoomInternetPlat()
{

}

/////////////////////////////////////////////////////////////////////
void LoomInternetPlat::print_config()
{
	LoomModule::print_config();	
}

/////////////////////////////////////////////////////////////////////
void LoomInternetPlat::print_state()
{
	Println3('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
}

