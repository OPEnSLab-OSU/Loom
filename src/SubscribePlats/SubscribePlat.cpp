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
  LMark;
	// check to see if we have a device manager
	if (!device_manager) {
   	LMark;
		print_module_label();
   	LMark;
		LPrint("No Device Manager!\n");
   	LMark;
		return;
	}

	LMark;
	m_internet = device_manager->get<InternetPlat>();
	LMark;
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
  LMark;
	Module::print_config();
}

///////////////////////////////////////////////////////////////////////////////
void SubscribePlat::print_state() const
{
  LMark;
	Module::print_state();
  LMark;
	LPrintln("\tInternet Connected: ", m_internet != nullptr && m_internet->is_connected());
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
bool SubscribePlat::subscribe()
{
  LMark;
	// call normal subscribe (implemented by derived classes)
	if (device_manager != nullptr) {
   	LMark;
		return subscribe( device_manager->internal_json(true) );  // true to clear json
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////

#endif // if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
