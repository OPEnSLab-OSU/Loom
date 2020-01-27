///////////////////////////////////////////////////////////////////////////////
///
/// @file		Power_Manager.cpp
/// @brief	Source file for Loom Module Power Manager.
/// @author		C. Greyston Brady
/// @date		  2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Power_Manager.h"
#include "Power.h"

Power_Manager::Power_Manager(
  LoomManager* manager
  )
  : LoomModule(manager, __FUNCTION__, LoomModule::Type::Power_Manager)
  , powered(true)
  {
    
}

Power_Manager::Power_Manager(
  LoomManager* manager,
  JsonArrayConst p
  ) 
  : Power_Manager(manager)
  {

}

void Power_Manager::Register_Power(Power* registree) {
  Interfaces.push_back(registree);
}

void Power_Manager::power_up() {
  for(auto interface : Interfaces) {
    interface->power_up();
  }
  powered = true;
}

void Power_Manager::power_down() {
  for(auto interface : Interfaces) {
    interface->power_down();
  }
  powered = false;
}