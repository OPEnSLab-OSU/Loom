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
  LMark;
	// check to see if we have a device manager
	if (device_manager == nullptr) {
   	LMark;
		print_module_label();
   	LMark;
		LPrint("No Device Manager!\n");
   	LMark;
		return;
	}

	LoomInternetPlat* temp = (LoomInternetPlat*)device_manager->find_module(internet_type);
  LMark;

	print_module_label();
  LMark;
	if (temp != nullptr && temp->get_module_type() != LoomModule::Type::Unknown) {
   	LMark;
		LPrintln("Found internet module: ", temp->get_module_name() , " (", (int)temp->get_module_type() , ")");
   	LMark;
		m_internet = temp;
  	LMark;
	}
	else {
   	LMark;
		LPrintln("Unable to find internet platform");
   	LMark;
		return;
	}

	// made it here, guess we're good to go!
	print_module_label();
  LMark;
	LPrint("Ready\n");
  LMark;

}

///////////////////////////////////////////////////////////////////////////////
void LoomSubscribePlat::print_config() const
{
  LMark;
	LoomModule::print_config();
  LMark;
	LPrintln("\tInternet Type: ", (int)internet_type);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomSubscribePlat::print_state() const
{
  LMark;
	LoomModule::print_state();
  LMark;
	LPrintln("\tInternet Connected: ", m_internet != nullptr && m_internet->is_connected());
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomSubscribePlat::subscribe()
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
