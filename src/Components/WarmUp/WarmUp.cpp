///////////////////////////////////////////////////////////////////////////////
///
/// @file		WarmUp.cpp
/// @brief	Source file for Loom Module Warmup mixin interface.
/// @author		C. Greyston Brady
/// @date		  2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "WarmUp.h"

#include "WarmUpManager.h"
#include "Manager.h"

using namespace Loom;

WarmUp::WarmUp(Manager* manager, int period)
  : period(period) {
    LMark;
    manager->get<WarmUpManager>()->Register_WarmUp(this);
  }

int WarmUp::get_period() {
  LMark;
  return this->period;
}
