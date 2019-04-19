
#include "Loom_Digital.h"

// Available digital pins 5, 6, 9, 10, 11, 12, A0(14), A1(15), A2(16), A3(17), A4(18), A5(19)
byte Loom_Digital::pin_nums[DIGITAL_COUNT] = {5, 6, 9, 10, 11, 12, 14, 15, 16, 17, 18, 19};

/////////////////////////////////////////////////////////////////////
Loom_Digital::Loom_Digital(	
		const char*		module_name, 

		bool			enable5,
		bool			enable6,
		bool			enable9,
		bool			enable10,
		bool			enable11,
		bool			enable12,

		bool			enableA0,
		bool			enableA1,
		bool			enableA2,
		bool			enableA3,
		bool			enableA4,
		bool			enableA5
	) 
	: LoomSensor( module_name, 1 )
{
	// Zero out array of measurements 
	for (int i = 0; i < DIGITAL_COUNT; i++) { 
		digital_vals[i] = 0; 
	}

	// Set enabled pins
	pin_enabled[0] = enable5;
	pin_enabled[1] = enable6;
	pin_enabled[2] = enable9;
	pin_enabled[3] = enable10;
	pin_enabled[4] = enable11;
	pin_enabled[5] = enable12;

	pin_enabled[6]  = enableA0;
	pin_enabled[7]  = enableA1;
	pin_enabled[8]  = enableA2;
	pin_enabled[9]  = enableA3;
	pin_enabled[10] = enableA4;
	pin_enabled[11] = enableA5;
}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_Digital::~Loom_Digital()
{

}

/////////////////////////////////////////////////////////////////////
void Loom_Digital::print_config()
{
	LoomSensor::print_config();

	Print("\tEnabled Pins        : ");
	// 5,6,9,10,11,12
	for (int i = 0; i < 6; i++) { 
		if (pin_enabled[i]) {
			Print2(pin_nums[i], ", ");
		}
	}
	// A0-A5
	for (int i = 0; i < 6; i++) { 
		if (pin_enabled[i+6]) {
			Print3("A", i, ", ");
		}
	}

	Println();
}

/////////////////////////////////////////////////////////////////////
void Loom_Digital::print_measurements()
{
	print_module_label();
	Println("Measurements:");
	// 5,6,9,10,11,12
	for (int i = 0; i < 6; i++) { 
		if (pin_enabled[i]) {
			Println4("\t", pin_nums[i], ": ", digital_vals[i]);
		}
	}
	// A0-A5
	for (int i = 0; i < 6; i++) { 
		if (pin_enabled[i+6]) {
			Println5("\t", "A", i, ": ", digital_vals[i+6]);
		}
	}

}

/////////////////////////////////////////////////////////////////////
void Loom_Digital::measure()
{
	for (int i = 0; i < DIGITAL_COUNT; i++) {
		digital_vals[i] = get_digital_val(pin_nums[i]);
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Digital::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	char buf[3];
	bool first = true;

	// 5,6,9,10,11,12
	for (int i = 0; i < 6; i++) { 
		if (pin_enabled[i]) {
			sprintf(buf, "%d", pin_nums[i]);
			append_to_bundle(bndl, id_prefix, buf, digital_vals[i], (!first) ? : NEW_MSG );
			first = false;
		}
	}
	// A0-A5
	for (int i = 0; i < 6; i++) { 
		if (pin_enabled[i+6]) {
			sprintf(buf, "%s%d", "A", i);
			append_to_bundle(bndl, id_prefix, buf, digital_vals[i+6], (!first) ? : NEW_MSG );
			first = false;
		}
	}
}

/////////////////////////////////////////////////////////////////////
bool Loom_Digital::get_digital_val(uint8_t pin)
{
	if (pin_enabled[ pin_to_index(pin) ]) {
		pinMode(pin, INPUT_PULLUP);
		return digitalRead(pin);
	} else {
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Digital::set_digital_val(uint8_t pin, bool state)
{
	if (pin_enabled[ pin_to_index(pin) ]) {
		pinMode(pin, OUTPUT);
		digitalWrite(pin, (state) ? HIGH : LOW ); 
	}	
}

/////////////////////////////////////////////////////////////////////
bool Loom_Digital::get_pin_enabled(uint8_t pin)
{
	return pin_enabled[ pin_to_index(pin) ];
}

/////////////////////////////////////////////////////////////////////
void Loom_Digital::set_pin_enabled(uint8_t pin, bool e)
{
	pin_enabled[ pin_to_index(pin) ] = e;
}

/////////////////////////////////////////////////////////////////////
uint8_t Loom_Digital::pin_to_index(uint8_t pin)
{
	for (int i = 0; i < DIGITAL_COUNT; i++) {
		if (pin_nums[i] == pin) return i;
	}
	return 0;
}





