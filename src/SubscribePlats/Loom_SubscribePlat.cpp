
#include "Loom_SubscribePlat.h"
#include "Loom_Manager.h"


///////////////////////////////////////////////////////////////////////////////
LoomSubscribePlat::LoomSubscribePlat(	
		const char*			module_name,
		LoomModule::Type	module_type,
		LoomModule::Type	internet_type
	) 
	: LoomModule( module_name, module_type )
	, m_internet( nullptr )
	, internet_type( internet_type ) 
{}

///////////////////////////////////////////////////////////////////////////////
void LoomSubscribePlat::second_stage_ctor() 
{
	// check to see if we have a device manager
	if (device_manager == nullptr) { 
		print_module_label(); 
		LPrint("No Device Manager!\n"); 
		return; 
	}

	// check if internet platform exists
	LoomInternetPlat* temp;
	switch (internet_type) {
		case LoomModule::Type::Ethernet: 
			temp = (LoomInternetPlat*)&(device_manager->Ethernet() );
			break;
		case LoomModule::Type::WiFi: 
			temp = (LoomInternetPlat*)&(device_manager->WiFi() );
			break;
		default:
			temp = nullptr;
	}
	
	print_module_label();
	if (temp != nullptr && temp->get_module_type() != LoomModule::Type::Unknown) {
		LPrintln("Found internet module: ", temp->get_module_name() , " (", (int)temp->get_module_type() , ")");
		m_internet = temp;
	}
	else {
		LPrintln("Unable to find internet platform");
		return;
	}

	// made it here, guess we're good to go!
	print_module_label();
	LPrint("Ready\n");

}

///////////////////////////////////////////////////////////////////////////////
void LoomSubscribePlat::print_config() 
{
	LoomModule::print_config();
	LPrintln("\tInternet Type: ", (int)internet_type);
}

///////////////////////////////////////////////////////////////////////////////
void LoomSubscribePlat::print_state() 
{
	LoomModule::print_state();
	LPrintln("\tInternet Connected: ", m_internet != nullptr && m_internet->is_connected());
}

///////////////////////////////////////////////////////////////////////////////
bool LoomSubscribePlat::subscribe()
{
	// call normal subscribe (implemented by derived classes)
	if (device_manager != nullptr) {
		return subscribe( device_manager->internal_json(true) );  // true to clear json
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
