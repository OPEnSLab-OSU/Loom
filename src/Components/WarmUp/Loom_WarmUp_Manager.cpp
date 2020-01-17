///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_WarmUp_Manager.cpp
/// @brief	Source file for Loom Module Warmup Manager.
/// @author		C. Greyston Brady
/// @date		  2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Loom_WarmUp_Manager.h"
#include "Loom_WarmUp.h"

Loom_WarmUp_Manager::Loom_WarmUp_Manager() 
  : LoomModule("WarmUp_Manager", LoomModule::Type::WarmUp_Manager)
  , warm(false)
  , start_time(0) {

}

Loom_WarmUp_Manager::Loom_WarmUp_Manager(JsonArrayConst) 
  : Loom_WarmUp_Manager() {

}

void Loom_WarmUp_Manager::warming_Begin() {
  start_time = millis();

  int warmupTime = 0;

  for(WarmUp* elem : Interfaces) {
    warmupTime = (warmupTime > elem->get_period()) ? warmupTime : elem->get_period();
  }

  return;
}