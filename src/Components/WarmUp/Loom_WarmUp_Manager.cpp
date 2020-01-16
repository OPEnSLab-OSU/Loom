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

Loom_WarmUp_Manager::Loom_WarmUp_Manager() 
  : LoomModule("WarmUp_Manager", LoomModule::Type::WarmUp_Manager) {

}

Loom_WarmUp_Manager::Loom_WarmUp_Manager(JsonArrayConst) 
  : Loom_WarmUp_Manager() {

}