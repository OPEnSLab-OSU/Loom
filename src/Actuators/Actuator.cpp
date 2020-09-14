///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Actuator.cpp
/// @brief		File for LoomActuator implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_ACTUATORS

#include "Actuator.h"

///////////////////////////////////////////////////////////////////////////////
LoomActuator::LoomActuator( 
		LoomManager* 			manager,
		const char*							module_name,
		const LoomModule::Type	module_type 
	) 
	: LoomModule(manager, module_name, module_type )
{}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_ACTUATORS
