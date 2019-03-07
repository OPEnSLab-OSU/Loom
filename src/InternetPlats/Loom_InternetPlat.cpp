
#include "Loom_InternetPlat.h"


char* LoomInternetPlat::enum_internet_plat_string(InternetPlatform p)
{
	switch(p) {
		case InternetPlatform::WIFI     : return "WiFi";
		case InternetPlatform::ETHERNET     : return "Ethernet";
		case InternetPlatform::CELLULAR_2G  : return "Cellular";
	}
}



// --- CONSTRUCTOR ---
LoomInternetPlat::LoomInternetPlat(	char* module_name

				) : LoomModule( module_name )
{

}

	// --- DESTRUCTOR ---
LoomInternetPlat::~LoomInternetPlat()
{

}


void LoomInternetPlat::print_config()
{
	LoomModule::print_config();	
}


void LoomInternetPlat::print_state()
{
	Println3('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
}

