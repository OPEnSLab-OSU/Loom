///////////////////////////////////////////////////////////////////////////////

// This example is used to demonstrate a few different use cases of the 
// processor's internal timers, manager via Loom

// In setup, there are 4 different timer configurations, uncomment one, and
// comment the rest out. The options are:
//		- Repeating immediate ISR timer
//		- Single immediate ISR timer
//		- Repeating delayed ISR timer
//		- Single delayed ISR timer

// Repeating means that the timer will restart after triggering, single will
// not.

// Immediate means that the ISR will run immediately when the timer elapses.

// Delayed means that a flag will be set when the timer elapses, but the ISR
// will not be run until you poll which flags have been set, with:
//		Loom.InterruptManager().run_pending_ISRs();
// This options is better if you have a large ISR, it contains delays, prints
// statements, or calls to copmlex functions

// The ISR being used for this example is toggle() below, which simply switches
// the state of the built-in LED when the timer elapses.

// For more details about the usage of the interrupt manager, see the 
// documentation: 
// https://openslab-osu.github.io/Loom/html/class_loom___interrupt___manager.html

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Enabled,
	Enable::Radios::Enabled,
	Enable::Actuators::Enabled,
	Enable::Max::Enabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };




volatile bool state = true;
volatile int count = 0;
void toggle()
{
	digitalWrite(LED_BUILTIN, state ? HIGH : LOW);
	state = !state;
	count++;
	LPrintln("Toggle");	// Generally not good practice to put a print in an ISR
						// it is only used here to make the program behavior 
						// more obvious
}


void setup() 
{ 
	Loom.begin_LED();
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();


	// Select on of the 4 configurations below:
	// If using 3 or 4, uncomment the 'run_pending_ISRs' line in loop()

	// 1)
	// Repeating alarm, runs ISR immediately on timer elapsed
	// Toggles the LED every 5 seconds
	Loom.InterruptManager().register_internal_timer(5, toggle, true, ISR_Type::IMMEDIATE);
	
	// 2)
	// Single alarm, runs ISR immediately on timer elapsed
	// Turns LED on after 5 seconds
	// Loom.InterruptManager().register_internal_timer(5, toggle, false, ISR_Type::IMMEDIATE);

	// 3)
	// Repeating alarm, runs ISR only after run_pending_ISRs called
	// Could also manually call .get_internal_timer_flag() and .clear_internal_timer_flag()
	// Loom.InterruptManager().register_internal_timer(5, toggle, true, ISR_Type::CHECK_FLAG);

	// 4)
	// Single alarm, runs ISR only after run_pending_ISRs called
	// Loom.InterruptManager().register_internal_timer(5, toggle, false, ISR_Type::CHECK_FLAG);


	// Start LED off
	digitalWrite(LED_BUILTIN, LOW);
	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	// Only needed with configuration 3 and 4
	// Loom.InterruptManager().run_pending_ISRs();

	// Demonstrating disabling alarm,
	// In this case, only after ISR triggers 5 times
	if (count > 4) {
		Loom.InterruptManager().internal_timer_enable(false);		
	}

}



