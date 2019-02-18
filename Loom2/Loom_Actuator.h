
#ifndef LOOM_ACTUATOR_h
#define LOOM_ACTUATOR_h

#include "Loom_Module.h"



class LoomActuator : public LoomModule
{

protected:

public:

	// --- CONSTRUCTOR ---
	LoomActuator( char* module_name );

	// --- DESTRUCTOR ---
	virtual ~LoomActuator();

	virtual void print_config();
	virtual void print_state() = 0;
	virtual void measure() {}
	virtual void package(OSCBundle& bndl, char* suffix="") = 0;
	virtual bool message_route(OSCMessage* msg, int address_offset) = 0;

private:

};


#endif
