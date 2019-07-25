#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

LoomManager Loom{};


volatile bool state = true;
volatile int count = 0;
void toggle()
{
	digitalWrite(LED_BUILTIN, state ? HIGH : LOW);
	state = !state;
	count++;
	LPrintln("Toggle");
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


