
#ifndef LOOM_INTERRUPT_MANAGER_h
#define LOOM_INTERRUPT_MANAGER_h

#include "Loom_Module.h"

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>



// Used to make function signatures easier to read 
// when returning function pointers
typedef void (*ISRFuncPtr)(); 



// INT_LOW		: 0
// INT_HIGH		: 1
// INT_RISING	: 4
// INT_FALLING	: 3
// INT_CHANGE 	: 2
enum IntType { INT_LOW, INT_HIGH, INT_RISING, INT_FALLING, INT_CHANGE };

struct IntDetails {
	byte pin;					// Pin Number
	IntType type;				// Interupt Type 
	ISRFuncPtr ISR_Bottom_Half;	// ISR Bottom Half

	// char[20] description ?
};


#define InteruptRange 16


class Loom_Interrupt_Manager : public LoomModule
{

protected:

	// Enable or disable all interrupts
	bool enable_interrupts;

	// Whether or not to call ISR on for interrupts on a given pin
	bool 		interrupt_enabled[InteruptRange];


	IntDetails 	interrupts[InteruptRange];
	
	// Flags set by interrupts, indicating ISR bottom 
	// half should be called if not Null
	bool 		interrupt_triggered[InteruptRange];

public:

	// --- CONSTRUCTOR ---
	Loom_Interrupt_Manager( char* 		module_name 	= "Interrupt_Manager"
							
							);


	// --- DESTRUCTOR ---
	~Loom_Interrupt_Manager();

	// General
	void print_config();
	void print_state();
	void measure() {}
	void package(OSCBundle& bndl, char* suffix="") {}
	bool message_route(OSCMessage& msg, int address_offset) {}

	// All interrupt enable
	void set_enable_interrupts(bool state);
	bool get_enable_interrupts();

	// Per interrupt enable
	void set_enable_interrupt(byte pin, bool state);
	bool get_enable_interrupt(byte pin);



	// Currently can give it any ISR to do whatever, rather than planned flag setter
	// Not sure which is ideal though
	// maybe have a register immediate interrupt vs register delayed bottom half?
	void register_interrupt_ISR(IntDetails details);
	void unregister_interrupt_ISR(byte pin, IntType=INT_LOW);  // restores to default ISR, does not disable interrupt


	void run_ISR_bottom_halfs();


	void interrupt_reset(byte pin);   // used to clear pending interrupts


	// Individually attach and detach interrupts?

private:


};


#endif // of #ifndef LOOM_INTERRUPT_MANAGER_h










