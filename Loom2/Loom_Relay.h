
#ifndef LOOM_RELAY_h
#define LOOM_RELAY_h


#include "Loom_Actuator.h"



class Loom_Relay : public LoomActuator
{

protected:

	byte pin;	// Which pin the relay is connected to
	bool on;	// Whether the relay is on or off

public:

	// --- CONSTRUCTOR ---
	Loom_Relay( char* module_name 	= "Relay",

				byte pin 			= 10
			   );

	// --- DESTRUCTOR ---
	virtual ~Loom_Relay();

	void print_config();
	void print_state();
	void package(OSCBundle& bndl, char* suffix="");

private:

};


#endif
