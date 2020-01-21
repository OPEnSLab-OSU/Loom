///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Actuator.h
/// @brief		File for LoomActuator definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Module.h"


///////////////////////////////////////////////////////////////////////////////
///
/// Abstract base class of actuator modules.
///
/// All actuator modules inherit from this class.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_actuator.html)
///
///////////////////////////////////////////////////////////////////////////////
class LoomActuator : public LoomModule
{

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// @param[in]	module_name		Name of the module (provided by derived classes)
	/// @param[in] 	module_type		Type of the module (provided by derived classes)
	LoomActuator( 
			LoomManager* 			manager,
			const char*							module_name, 
			const LoomModule::Type	module_type
		);

	/// Destructor
	virtual ~LoomActuator() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// Not all actuators need a package method, implement with empty body
	virtual void 	package(JsonObject json) override {}

private:

};

