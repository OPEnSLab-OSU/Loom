
#include "Loom_Interrupt_Manager.h"

// #include "Loom_Sleep_Manager.h"
// #include "Loom_RTC.h"

// #include <Adafruit_SleepyDog.h>
// #include <LowPower.h>



bool Loom_Interrupt_Manager::interrupt_triggered[InteruptRange] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


const ISRFuncPtr Loom_Interrupt_Manager::default_ISRs[InteruptRange] =
{
	default_ISR_0,  default_ISR_1,  default_ISR_2,  default_ISR_3,
	default_ISR_4,  default_ISR_5,  default_ISR_6,  default_ISR_7,
	default_ISR_8,  default_ISR_9,  default_ISR_10, default_ISR_11,
	default_ISR_12, default_ISR_13, default_ISR_14, default_ISR_15
};




Loom_Interrupt_Manager::Loom_Interrupt_Manager( char* module_name) 
	: LoomModule( module_name )
{

	interrupts_enabled = true;

	for (int i = 0; i < InteruptRange; i++) {
		settings[i] = {NULL, 0, true, false};
	}
}


// --- DESTRUCTOR ---
Loom_Interrupt_Manager::~Loom_Interrupt_Manager()
{

}

void Loom_Interrupt_Manager::print_config()
{
	LoomModule::print_config();

	Println3('\t', "Interrupts Enabled  : ", (interrupts_enabled) ? "True" : "False" );

	// print out registered interrupts
	Println2('\t', "Registered ISRs     : " );
	for (int i = 0; i < InteruptRange; i++) {
		Print5("\t\t", "Pin ", i, " | ISR: ", (settings[i].is_immediate) ? "Immediate" : "Bottom Half");
		Println4(" | Type: ", settings[i].type, " | ", (settings[i].is_enabled) ? "Enabled" : "Disabled" );

	}
}


void Loom_Interrupt_Manager::print_state()
{
	LoomModule::print_state();


}



void Loom_Interrupt_Manager::set_interrupts_enabled(bool state)
{
	interrupts_enabled = state;
}

bool Loom_Interrupt_Manager::get_interrupts_enabled()
{
	return interrupts_enabled;
}


void Loom_Interrupt_Manager::set_enable_interrupt(byte pin, bool state)
{
	if (pin < InteruptRange) {
		settings[pin].is_enabled = state;
	} 
}

bool Loom_Interrupt_Manager::get_enable_interrupt(byte pin)
{
	return (pin < InteruptRange) ? settings[pin].is_enabled : false; 
}



void Loom_Interrupt_Manager::register_ISR(byte pin, ISRFuncPtr ISR, byte type, bool immediate)
{
	if (pin < InteruptRange) {

		// Save interrupt details
		settings[pin] = { ISR, type, immediate, (ISR) ? true : false };


		// Set pin mode
		pinMode(pin, INPUT_PULLUP);



		// If ISR provided
		if (ISR != NULL) {

			ISRFuncPtr tmpISR = (immediate) ? ISR : default_ISRs[pin];

			// Attach default interrupt with specified type
			// switch( type ) {
			// 	case INT_LOW     : attachInterrupt(digitalPinToInterrupt(pin), tmpISR, LOW); break;
			// 	case INT_HIGH    : attachInterrupt(digitalPinToInterrupt(pin), tmpISR, HIGH); break; 
			// 	case INT_CHANGE  : attachInterrupt(digitalPinToInterrupt(pin), tmpISR, CHANGE); break;
			// 	case INT_FALLING : attachInterrupt(digitalPinToInterrupt(pin), tmpISR, FALLING); break;
			// 	case INT_RISING  : attachInterrupt(digitalPinToInterrupt(pin), tmpISR, RISING); break;
			// }
			attachInterrupt(digitalPinToInterrupt(pin), tmpISR, (type<5) ? type : 0 );


		} 
		// If no ISR, detach interrupt pin
		else {
			detachInterrupt(digitalPinToInterrupt(pin));
		}

		// Ensure triggered flag false 
		interrupt_triggered[pin] = false;
	}

}






void Loom_Interrupt_Manager::unregister_ISR(byte pin, byte type)
{
	// Set interrupt to be the default
	if (pin < InteruptRange) {
		register_ISR(pin, NULL, type, true);
	}
}




void Loom_Interrupt_Manager::run_ISR_bottom_halves()
{
	if (interrupts_enabled) {

		// For each interrupt pin
		for (int i = 0; i < InteruptRange; i++) {

			// If pin enabled and bottom half ISR provided
			// ISR will be Null if no interrupt on that pin,
			// or if ISR on pin is set to immediate (dont want to rerun the ISR here)
			if ( (interrupt_triggered[i]) && (settings[i].ISR != NULL) ) {

				Println2("I: ", i);


				// Run bottom half ISR
				settings[i].ISR();

				// Reattach interrupts (disconnected in default ISR top half)
				// switch( settings[i].type ) {
				// 	case INT_LOW     : attachInterrupt(digitalPinToInterrupt(i), default_ISRs[i], LOW); break;
				// 	case INT_HIGH    : attachInterrupt(digitalPinToInterrupt(i), default_ISRs[i], HIGH); break; 
				// 	case INT_CHANGE  : attachInterrupt(digitalPinToInterrupt(i), default_ISRs[i], CHANGE); break;
				// 	case INT_FALLING : attachInterrupt(digitalPinToInterrupt(i), default_ISRs[i], FALLING); break;
				// 	case INT_RISING  : attachInterrupt(digitalPinToInterrupt(i), default_ISRs[i], RISING); break;
				// }

				// // set triggered flag false 
				interrupt_triggered[i] = false;
			}
							// set triggered flag false 
				// interrupt_triggered[i] = false;

		}
	}
}



void Loom_Interrupt_Manager::interrupt_reset(byte pin)
{
	detachInterrupt(digitalPinToInterrupt(pin));
	delay(20);

	if (settings[pin].ISR != NULL) {

		// Attach interrupt with specified type
		// switch( settings[pin].type ) {
		// 	case INT_LOW     : attachInterrupt(digitalPinToInterrupt(pin), settings[pin].ISR, LOW);
		// 	case INT_HIGH    : attachInterrupt(digitalPinToInterrupt(pin), settings[pin].ISR, HIGH); 
		// 	case INT_RISING  : attachInterrupt(digitalPinToInterrupt(pin), settings[pin].ISR, RISING);
		// 	case INT_FALLING : attachInterrupt(digitalPinToInterrupt(pin), settings[pin].ISR, FALLING);
		// 	case INT_CHANGE  : attachInterrupt(digitalPinToInterrupt(pin), settings[pin].ISR, CHANGE);
		// }
		attachInterrupt(digitalPinToInterrupt(pin), settings[pin].ISR, (settings[pin].type<5) ? settings[pin].type : 0 );

	}


}



