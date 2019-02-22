
#ifndef LOOM_INTERRUPT_MANAGER_h
#define LOOM_INTERRUPT_MANAGER_h

#include "Loom_Module.h"

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>



// Used to make function signatures easier to read 
// when returning function pointers
typedef void (*ISRFuncPtr)(); 

enum IntType { INT_LOW, INT_HIGH, INT_RISING, INT_FALLING, INT_CHANGE };

struct IntDetails {
	byte pin;					// Pin Number
	IntType type;				// Interupt Type 
	ISRFuncPtr ISR_Bottom_Half;	// ISR Bottom Half

	// char[20] description ?
};





class Loom_Interrupt_Manager : public LoomModule
{

protected:

	bool enable_interrupts;

public:

	// --- CONSTRUCTOR ---
	Loom_Interrupt_Manager( char* 		module_name 	= "Interrupt_Manager"
							

					);

	// Loom_Interrupt_Manager( char* module_name, LoomManager* LD );


	// --- DESTRUCTOR ---
	~Loom_Interrupt_Manager();

	// General
	void print_config();
	void print_state();
	void measure() {}
	void package(OSCBundle& bndl, char* suffix="") {}
	bool message_route(OSCMessage& msg, int address_offset) {}



	void set_enable_interrupts(bool state);
	bool get_enable_interrupts();


	// Currently can give it any ISR to do whatever, rather than planned flag setter
	// Not sure which is ideal though
	// maybe have a register immediate interrupt vs register delayed bottom half?
	void register_interrupt(IntDetails details);

	// Individually attach and detach interrupts?

private:


};


#endif // of #ifndef LOOM_INTERRUPT_MANAGER_h










