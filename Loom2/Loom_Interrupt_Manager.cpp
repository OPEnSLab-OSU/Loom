
#include "Loom_Interrupt_Manager.h"

// #include "Loom_Sleep_Manager.h"
// #include "Loom_RTC.h"

// #include <Adafruit_SleepyDog.h>
// #include <LowPower.h>







Loom_Interrupt_Manager::Loom_Interrupt_Manager( char* module_name) 
	: LoomModule( module_name )
{

}


// --- DESTRUCTOR ---
Loom_Interrupt_Manager::~Loom_Interrupt_Manager()
{

}

void Loom_Interrupt_Manager::print_config()
{
	LoomModule::print_config();

}


void Loom_Interrupt_Manager::print_state()
{
	LoomModule::print_state();

	// print out registered interrupts

}



void Loom_Interrupt_Manager::set_enable_interrupts(bool state)
{
	enable_interrupts = state;
}

bool Loom_Interrupt_Manager::get_enable_interrupts()
{
	return enable_interrupts;
}



void Loom_Interrupt_Manager::register_interrupt_ISR(IntDetails details)
{
	interrupts[details.pin] = details;

	// Set pin mode
	pinMode(details.pin, OUTPUT);

	// Attach interrupt with specified type
	switch( details.type ) {
		case INT_LOW     : attachInterrupt(digitalPinToInterrupt(details.pin), details.ISR_Bottom_Half, LOW);
		case INT_HIGH    : attachInterrupt(digitalPinToInterrupt(details.pin), details.ISR_Bottom_Half, HIGH); 
		case INT_RISING  : attachInterrupt(digitalPinToInterrupt(details.pin), details.ISR_Bottom_Half, RISING);
		case INT_FALLING : attachInterrupt(digitalPinToInterrupt(details.pin), details.ISR_Bottom_Half, FALLING);
		case INT_CHANGE  : attachInterrupt(digitalPinToInterrupt(details.pin), details.ISR_Bottom_Half, CHANGE);
	}

}



void Loom_Interrupt_Manager::run_ISR_bottom_halfs()
{
	for (int i = 0; i < InteruptRange; i++) {
		if ( (interrupt_triggered[i]) && (interrupts[i].ISR_Bottom_Half != NULL) ) {
			interrupts[i].ISR_Bottom_Half();
		}
	}
}




void Loom_Interrupt_Manager::interrupt_reset(byte pin)
{
	detachInterrupt(digitalPinToInterrupt(pin));
	delay(20);

	// Attach interrupt with specified type
	switch( interrupts[pin].type ) {
		case INT_LOW     : attachInterrupt(digitalPinToInterrupt(pin), interrupts[pin].ISR_Bottom_Half, LOW);
		case INT_HIGH    : attachInterrupt(digitalPinToInterrupt(pin), interrupts[pin].ISR_Bottom_Half, HIGH); 
		case INT_RISING  : attachInterrupt(digitalPinToInterrupt(pin), interrupts[pin].ISR_Bottom_Half, RISING);
		case INT_FALLING : attachInterrupt(digitalPinToInterrupt(pin), interrupts[pin].ISR_Bottom_Half, FALLING);
		case INT_CHANGE  : attachInterrupt(digitalPinToInterrupt(pin), interrupts[pin].ISR_Bottom_Half, CHANGE);
	}

}
