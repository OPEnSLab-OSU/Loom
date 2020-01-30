///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_WarmUp_Manager.cpp
/// @brief	Source file for Loom Module Warmup Manager.
/// @author		C. Greyston Brady
/// @date		  2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "WarmUp_Manager.h"
#include "WarmUp.h"

Loom_WarmUp_Manager::Loom_WarmUp_Manager(
    LoomManager* manager
  ) 
  : LoomModule(manager, "WarmUp_Manager", LoomModule::Type::WarmUp_Manager)
  , warm(false)
  , start_time(0)
  , warm_duration(0) {

}

Loom_WarmUp_Manager::Loom_WarmUp_Manager(LoomManager* manager, JsonArrayConst) 
  : Loom_WarmUp_Manager(manager) {

}

void Loom_WarmUp_Manager::warming_Begin() {
  start_time = millis();

  for(WarmUp* elem : Interfaces) {
    warm_duration = (warm_duration > elem->get_period()) ? warm_duration : elem->get_period();
  }

  return;
}

void Loom_WarmUp_Manager::warming_Reset() {
  warm = false;
  start_time = 0;
  return;
}

bool Loom_WarmUp_Manager::is_warm() {
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