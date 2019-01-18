
#include "Loom_InternetPlat.h"
#include "Loom_Device.h"
#include "Loom_Macros.h"




char* LoomInternetPlat::enum_internet_plat_string(InternetPlatform p)
{
	switch(p) {
		case INT_WIFI     : return "WiFi";
		case ETHERNET     : return "Ethernet";
		case CELLULAR_2G  : return "Cellular";
	}
}



// --- CONSTRUCTOR ---
LoomInternetPlat::LoomInternetPlat(	char* module_name

				)
{

}

	// --- DESTRUCTOR ---
LoomInternetPlat::~LoomInternetPlat()
{

}


void LoomInternetPlat::print_config()
{

}


bool LoomInternetPlat::log_to_pushingbox(OSCBundle* bndl)
{
	
}
