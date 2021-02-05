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

using namespace Loom;

WarmUpManager::WarmUpManager(
  )
  : Module("WarmUp_Manager")
  , warm(false)
  , start_time(0)
  , warm_duration(0) {
  LMark;

}

WarmUpManager::WarmUpManager(JsonArrayConst p)
  : WarmUpManager() {}

void WarmUpManager::warming_Begin() {
  LMark;
  start_time = millis();
  LMark;

  for(WarmUp* elem : Interfaces) {
    LMark;
    warm_duration = (warm_duration > elem->get_period()) ? warm_duration : elem->get_period();
    LMark;
  }

  return;
}

void WarmUpManager::warming_Reset() {
  LMark;
  warm = false;
  LMark;
  start_time = 0;
  LMark;
  return;
}

bool WarmUpManager::is_warm() {
  LMark;
  if(!warm) {
    LMark;
    return false;
  } else {
    if(millis() >= start_time+warm_duration) {
      LMark;
      return warm = true;;
    } else {
      return false;
    }
  }
}
