#pragma once

#include "Loom_Module.h"


/// Abstract base class of actuator modules
class LoomActuator : public LoomModule
{

protected:

public:

	// Constructor
	LoomActuator( 
			const char*		module_name 
		);

	/// Destructor
	virtual ~LoomActuator();

	// Inherited Methods
	virtual void	print_config() override;
	virtual void	print_state() = 0;
	virtual void	measure() {}
	virtual bool	cmd_route(JsonObject) = 0;
	virtual void 	package(JsonObject json) {}

private:

};

