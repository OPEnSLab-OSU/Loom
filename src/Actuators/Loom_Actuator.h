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
			const char*		module_name 
		);

	/// Destructor
	virtual ~LoomActuator() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	virtual bool	cmd_route(JsonObject) override = 0;
	virtual void 	package(JsonObject json) override {}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() override;
	virtual void	print_state() override = 0;

private:

};

