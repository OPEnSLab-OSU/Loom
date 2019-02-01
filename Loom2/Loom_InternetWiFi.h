
#ifndef LOOM_INTERNET_WIFI_PLAT_h
#define LOOM_INTERNET_WIFI_PLAT_h

#include "Loom_InternetPlat.h"





class Loom_WiFi_I : public LoomInternetPlat
{

protected:
	// --- PROTECTED MEMBERS ---
	
	
public:

	// --- PUBLIC MEMBERS ---


	// --- CONSTRUCTOR ---
	Loom_WiFi_I(	char* module_name

					);

	// --- DESTRUCTOR ---
	~Loom_WiFi_I();


	// --- PUBLIC METHODS ---

	void print_config();
	void print_state();

	bool connect();
	bool is_connected();
	uint32_t get_time();



	// Should this be external?
	bool log_to_pushingbox(OSCBundle* bndl);

private:

// --- PRIVATE MEMBERS ---

// --- PRIVATE METHODS --- 



};



#endif

