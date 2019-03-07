
#include "Loom_Fona.h"


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_Fona::Loom_Fona(	char* module_name,

						char* apn
						
				) : LoomInternetPlat( module_name )
{
	fona = new Adafruit_FONA(FONA_RST);		// Use this for FONA 800 and 808s

	fonaSerial = &Serial1; 	// used to communicate with Fona board




}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_Fona::~Loom_Fona()
{
	delete fona;
}

/////////////////////////////////////////////////////////////////////
void Loom_Fona::print_config()
{
	LoomInternetPlat::print_config();
	
	// Println2('\t', "IP:                 : " );	
}

/////////////////////////////////////////////////////////////////////
void Loom_Fona::print_state()
{
	LoomInternetPlat::print_state();	
	// Println3('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
}

/////////////////////////////////////////////////////////////////////
bool Loom_Fona::connect()
{

}

/////////////////////////////////////////////////////////////////////
bool Loom_Fona::is_connected()
{


}



