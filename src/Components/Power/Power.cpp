///////////////////////////////////////////////////////////////////////////////
///
/// @file		WarmUp.cpp
/// @brief	Source file for Loom Module Power mixin interface.
/// @author		C. Greyston Brady
/// @date		  2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Power.h"

#include "Power_Manager.h"
#include "Manager.h"

Power::Power(LoomManager* manager)
  : powered(false)
  {
    static_cast<Power_Manager*>(manager->find_module(LoomModule::Type::Power_Manager))->Register_Power(this);
}

bool Power::get_power() {
  return this->powered;
}