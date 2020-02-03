///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_WarmUp.cpp
/// @brief	Source file for Loom Module Warmup mixin interface.
/// @author		C. Greyston Brady
/// @date		  2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "WarmUp.h"

#include "WarmUp_Manager.h"
#include "Manager.h"

WarmUp::WarmUp(LoomManager* manager, int period) 
  : period(period) {
    static_cast<Loom_WarmUp_Manager*>(manager->find_module(LoomModule::Type::WarmUp_Manager))->Register_WarmUp(this);
  }

int WarmUp::get_period() {
  return this->period;
}