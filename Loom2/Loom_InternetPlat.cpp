
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
	LOOM_DEBUG_Println3('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
}


bool LoomInternetPlat::log_to_pushingbox(OSCBundle* bndl)
{
	
}
