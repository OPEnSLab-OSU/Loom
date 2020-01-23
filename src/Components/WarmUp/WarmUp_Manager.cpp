///////////////////////////////////////////////////////////////////////////////
///
/// @file		WarmUp_Manager.cpp
/// @brief	Source file for Loom Module Warmup Manager.
/// @author		C. Greyston Brady
/// @date		  2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "WarmUp_Manager.h"
#include "WarmUp.h"

WarmUp_Manager::WarmUp_Manager(
    LoomManager* manager
  ) 
  : LoomModule(manager, __FUNCTION__, LoomModule::Type::WarmUp_Manager)
  , warm(false)
  , start_time(0)
  , warm_duration(0) {

}

WarmUp_Manager::WarmUp_Manager(LoomManager* manager, JsonArrayConst) 
  : WarmUp_Manager(manager) {

}

void WarmUp_Manager::Register_WarmUp(WarmUp* registree){
  Interfaces.push_back(static_cast<WarmUp*>(registree));
}

void WarmUp_Manager::warming_Begin() {
  start_time = millis();

  for(WarmUp* elem : Interfaces) {
    warm_duration = (warm_duration > elem->get_period()) ? warm_duration : elem->get_period();
  }

  return;
}

void WarmUp_Manager::warming_Reset() {
  warm = false;
  start_time = 0;
  return;
}

bool WarmUp_Manager::is_warm() {
  if(!warm) {
    return false;
  } else {
    if(millis() >= start_time+warm_duration) {
      return warm = true;;
    } else {
      return false;
    }
  }
}