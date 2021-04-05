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
		const char*							module_name
	)
	: Module(module_name)
	, m_internet( nullptr )
{}

///////////////////////////////////////////////////////////////////////////////
void SubscribePlat::second_stage_ctor()
{
	// check to see if we have a device manager
	if (!device_manager) {
		print_module_label();
		LPrint("No Device Manager!\n");
		return;
	}
	m_internet = device_manager->get<InternetPlat>();
	if (!m_internet) {
		print_module_label();
		LPrintln("Unable to find internet platform");
	}

	// made it here, guess we're good to go!
	print_module_label();
	LPrint("Ready\n");

}

///////////////////////////////////////////////////////////////////////////////
void SubscribePlat::print_config() const
{
	Module::print_config();
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
