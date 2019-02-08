
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

	// --- PUBLIC METHODS ---

	virtual void print_config();
	virtual void print_state() = 0;
	virtual void measure() {}
	virtual void package(OSCBundle* bndl) = 0;

private:

};


#endif