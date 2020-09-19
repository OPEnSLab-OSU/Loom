///////////////////////////////////////////////////////////////////////////////
///
/// @file		Digital.cpp
/// @brief		File for Digital implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Digital.h"
#include "Module_Factory.h"

using namespace Loom;

// Available digital pins 5, 6, 9, 10, 11, 12, A0(14), A1(15), A2(16), A3(17), A4(18), A5(19)
const uint8_t Digital::pin_nums[DIGITAL_COUNT] = {5, 6, 9, 10, 11, 12, 14, 15, 16, 17, 18, 19};

///////////////////////////////////////////////////////////////////////////////

REGISTER(Module, Digital, "Digital");

///////////////////////////////////////////////////////////////////////////////
Digital::Digital(	
		const bool		enable5,
		const bool		enable6,
		const bool		enable9,
		const bool		enable10,
		const bool		enable11,
		const bool		enable12,

		const bool		enableA0,
		const bool		enableA1,
		const bool		enableA2,
		const bool		enableA3,
		const bool		enableA4,
		const bool		enableA5
	) 
	: Sensor("Digital", Type::Digital, 1)
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


///////////////////////////////////////////////////////////////////////////////
Digital::Digital(JsonArrayConst p)
	: Digital(EXPAND_ARRAY(p, 12) ) {}

///////////////////////////////////////////////////////////////////////////////
void Digital::add_config(JsonObject json)
{
	// add_config_aux(json, module_name,
	// 	module_name,
	// 	pin_enabled[0], pin_enabled[1], pin_enabled[2], pin_enabled[3], pin_enabled[4], pin_enabled[5],
	// 	pin_enabled[6], pin_enabled[7], pin_enabled[8], pin_enabled[9], pin_enabled[10], pin_enabled[11]
	// );
	JsonArray params = add_config_temp(json, module_name);
	params.add(module_name);
	for (int i = 0; i < 12; i++) {
		params.add(pin_enabled[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
void Digital::print_config() const
{
	Sensor::print_config();

	LPrint("\tEnabled Pins        : ");
	// 5,6,9,10,11,12
	for (int i = 0; i < 6; i++) { 
		if (pin_enabled[i]) {
			LPrint(pin_nums[i], ", ");
		}
	}
	// A0-A5
	for (int i = 0; i < 6; i++) { 
		if (pin_enabled[i+6]) {
			LPrint("A", i, ", ");
		}
	}

	LPrintln();
}

///////////////////////////////////////////////////////////////////////////////
void Digital::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	// 5,6,9,10,11,12
	for (int i = 0; i < 6; i++) { 
		if (pin_enabled[i]) {
			LPrintln("\t", pin_nums[i], ": ", digital_vals[i]);
		}
	}
	// A0-A5
	for (int i = 0; i < 6; i++) { 
		if (pin_enabled[i+6]) {
			LPrintln("\t", "A", i, ": ", digital_vals[i+6]);
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
void Digital::measure()
{
	for (int i = 0; i < DIGITAL_COUNT; i++) {
		digital_vals[i] = get_digital_val(pin_nums[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
void Digital::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	char buf[3];

	// 5,6,9,10,11,12
	for (auto i = 0; i < 6; i++) { 
		if (pin_enabled[i]) {
			sprintf(buf, "%d", pin_nums[i]);
			data[buf] = digital_vals[i];
		}
	}
	// A0-A5
	for (auto i = 0; i < 6; i++) { 
		if (pin_enabled[i+6]) {
			sprintf(buf, "%s%d", "A", i);
			data[buf] = digital_vals[i];
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Digital::get_digital_val(const uint8_t pin) const
{
	if (pin_enabled[ pin_to_index(pin) ]) {
		pinMode(pin, INPUT_PULLUP);
		return digitalRead(pin);
	} else {
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Digital::set_digital_val(const uint8_t pin, const bool state)
{
	if (pin_enabled[ pin_to_index(pin) ]) {
		pinMode(pin, OUTPUT);
		digitalWrite(pin, (state) ? HIGH : LOW ); 
	}	
}

///////////////////////////////////////////////////////////////////////////////
bool Digital::get_pin_enabled(const uint8_t pin) const
{
	return pin_enabled[ pin_to_index(pin) ];
}

///////////////////////////////////////////////////////////////////////////////
void Digital::set_pin_enabled(const uint8_t pin, const bool e)
{
	pin_enabled[ pin_to_index(pin) ] = e;
}

///////////////////////////////////////////////////////////////////////////////
uint8_t Digital::pin_to_index(const uint8_t pin) const
{
	for (int i = 0; i < DIGITAL_COUNT; i++) {
		if (pin_nums[i] == pin) return i;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////




