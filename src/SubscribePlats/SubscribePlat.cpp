///////////////////////////////////////////////////////////////////////////////
///
/// @file		SubscribePlat.cpp
/// @brief		File for SubscribePlat implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))

#include "SubscribePlat.h"
#include "Manager.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
SubscribePlat::SubscribePlat(	
		const char*				module_name,
		const Module::Type	module_type,
		const Module::Type	internet_type
	) 
	: Module(module_name, module_type)
	, m_internet( nullptr )
	, internet_type( internet_type ) 
{}

///////////////////////////////////////////////////////////////////////////////
void SubscribePlat::second_stage_ctor() 
{
	// check to see if we have a device manager
	if (device_manager == nullptr) { 
		print_module_label(); 
		LPrint("No Device Manager!\n"); 
		return; 
	}

	InternetPlat* temp = (InternetPlat*)device_manager->find_module(internet_type);

	print_module_label();
	if (temp != nullptr && temp->get_module_type() != Module::Type::Unknown) {
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
void SubscribePlat::print_config() const
{
	Module::print_config();
	LPrintln("\tInternet Type: ", (int)internet_type);
}

///////////////////////////////////////////////////////////////////////////////
void SubscribePlat::print_state() const
{
	Module::print_state();
	LPrintln("\tInternet Connected: ", m_internet != nullptr && m_internet->is_connected());
}

///////////////////////////////////////////////////////////////////////////////
bool SubscribePlat::subscribe()
{
	// call normal subscribe (implemented by derived classes)
	if (device_manager != nullptr) {
		return subscribe( device_manager->internal_json(true) );  // true to clear json
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////

#endif // if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
