///////////////////////////////////////////////////////////////////////////////
///
/// @file		WarmUpManager.cpp
/// @brief	Source file for Loom Module Warmup Manager.
/// @author		C. Greyston Brady
/// @date		  2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "WarmUpManager.h"
#include "WarmUp.h"

WarmUpManager::WarmUpManager() 
  : LoomModule("WarmUpManager", LoomModule::Type::WarmUpManager)
  , warm(false)
  , start_time(0)
  , warm_duration(0) {

}

WarmUpManager::WarmUpManager(JsonArrayConst p) 
  : WarmUpManager() {}

void WarmUpManager::warming_Begin() {
  start_time = millis();

  for(WarmUp* elem : Interfaces) {
    warm_duration = (warm_duration > elem->get_period()) ? warm_duration : elem->get_period();
  }

  return;
}

void WarmUpManager::warming_Reset() {
  warm = false;
  start_time = 0;
  return;
}

bool WarmUpManager::is_warm() {
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