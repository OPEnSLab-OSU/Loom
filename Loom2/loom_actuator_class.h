
#ifndef LOOM_ACTUATOR_h
#define LOOM_ACTUATOR_h


#include "loom_abstract_module_class.h"

#include <OSCBundle.h>



class LoomActuator : public LoomModule
{

protected:

public:

	// --- CONSTRUCTOR ---
	LoomActuator( char* module_name );

	// --- DESTRUCTOR ---
	~LoomActuator();


	// --- PUBLIC METHODS ---

	virtual void print_config();

	virtual void print_state() = 0;

	virtual void measure();

	virtual void package(OSCBundle* bndl, char* id_prefix) = 0;

private:

};


#endif