///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_SubscribePlat.cpp
/// @brief		File for LoomSubscribePlat implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "SubscribePlat.h"
#include "Manager.h"

///////////////////////////////////////////////////////////////////////////////
LoomSubscribePlat::LoomSubscribePlat(	
		LoomManager* 			manager,
		const char*							module_name,
		const LoomModule::Type	module_type,
		const LoomModule::Type	internet_type
	) 
	: LoomModule(manager, module_name, module_type )
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
	
	m_internet = (LoomInternetPlat*)device_manager->find_module_by_category(LoomModule::Category::InternetPlat);

	if (!m_internet) {
		print_module_label();
		LPrintln("Unable to find internet platform");
	}


	// made it here, guess we're good to go!
	print_module_label();
	LPrint("Ready\n");

}

///////////////////////////////////////////////////////////////////////////////
void LoomSubscribePlat::print_config() const
{
	LoomModule::print_config();
	LPrintln("\tInternet Type: ", (int)internet_type);
}

///////////////////////////////////////////////////////////////////////////////
void LoomSubscribePlat::print_state() const
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
