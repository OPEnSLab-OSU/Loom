#pragma once

#include "Loom_Module.h"


///////////////////////////////////////////////////////////////////////////////


/// Abstract base class of actuator modules
class LoomActuator : public LoomModule
{

protected:

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomActuator( 
			const char*			module_name, 
			LoomModule::Type	module_type
		);

	/// Destructor
	virtual ~LoomActuator() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

    void Run() {}
	virtual bool	dispatch(JsonObject) override = 0;
	virtual void 	package(JsonObject json) override {}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_state() override = 0;

private:

};

