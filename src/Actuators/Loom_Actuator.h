///////////////////////////////////////////////////////////////////////////////

/// Abstract base class of actuator modules.

/// All actuator modules inherit from this class.

///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Loom_Module.h"


///////////////////////////////////////////////////////////////////////////////


class LoomActuator : public LoomModule
{

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// \param[in]	module_name		Name of the module (provided by derived classes)
	/// \param[in] 	module_type		Type of the module (provided by derived classes)
	LoomActuator( 
			const char*			module_name, 
			LoomModule::Type	module_type
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

