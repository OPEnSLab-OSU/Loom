
#ifndef LOOM_INTERRUPT_MANAGER_h
#define LOOM_INTERRUPT_MANAGER_h

#include "Loom_Module.h"

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>



class Loom_Interrupt_Manager : public LoomModule
{

protected:



public:

	// --- CONSTRUCTOR ---
	Loom_Interrupt_Manager( char* 		module_name 	= "Interrupt_Manager"
							

					);

	// Loom_Interrupt_Manager( char* module_name, LoomManager* LD );


	// --- DESTRUCTOR ---
	~Loom_Interrupt_Manager();


	void print_config();
	void print_state();
	void measure() {}
	void package(OSCBundle& bndl, char* suffix="") {}
	bool message_route(OSCMessage& msg, int address_offset) {}






private:


};


#endif // of #ifndef LOOM_INTERRUPT_MANAGER_h










