
#include "Loom_InternetWiFi.h"




// --- CONSTRUCTOR ---
Loom_WiFi_I::Loom_WiFi_I(	char* module_name

				) : LoomInternetPlat( module_name )
{

}

	// --- DESTRUCTOR ---
Loom_WiFi_I::~Loom_WiFi_I()
{

}


void Loom_WiFi_I::print_config()
{
	LoomInternetPlat::print_config();	
}


void Loom_WiFi_I::print_state()
{
	LoomInternetPlat::print_state();	
	// LOOM_DEBUG_Println3('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
}


// bool Loom_WiFi_I::log_to_pushingbox(OSCBundle* bndl)
// {
	
// }
