#pragma once

#include "Loom_Module.h"


/// Abstract base class of actuator modules
class LoomActuator : public LoomModule
{

protected:

public:

	// --- CONSTRUCTOR ---
	LoomActuator( 
			const char*		module_name 
		);

	// --- DESTRUCTOR ---
	virtual ~LoomActuator();

	// Inherited Methods
	virtual void	print_config() override;
	virtual void	print_state() = 0;
	virtual void	measure() {}
	// virtual bool	message_route(OSCMessage& msg, int address_offset) = 0;
	virtual bool	cmd_route(JsonObject) = 0;
	virtual void 	package(JsonObject json) {}

private:

};

