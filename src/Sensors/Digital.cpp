///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Digital.cpp
/// @brief		File for Loom_Digital implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Digital.h"

// Available digital pins 5, 6, 9, 10, 11, 12, A0(14), A1(15), A2(16), A3(17), A4(18), A5(19)
const uint8_t Loom_Digital::pin_nums[DIGITAL_COUNT] = {5, 6, 9, 10, 11, 12, 14, 15, 16, 17, 18, 19};


///////////////////////////////////////////////////////////////////////////////
Loom_Digital::Loom_Digital(
		LoomManager* manager,
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
	: LoomSensor(manager, "Digital", Type::Digital, 1 )
{
  LMark;
	// Zero out array of measurements
	for (int i = 0; i < DIGITAL_COUNT; i++) {
   	LMark;
		digital_vals[i] = 0;
  	LMark;
	}

	// Set enabled pins
	pin_enabled[0] = enable5;
  LMark;
	pin_enabled[1] = enable6;
  LMark;
	pin_enabled[2] = enable9;
  LMark;
	pin_enabled[3] = enable10;
  LMark;
	pin_enabled[4] = enable11;
  LMark;
	pin_enabled[5] = enable12;
  LMark;

	pin_enabled[6]  = enableA0;
  LMark;
	pin_enabled[7]  = enableA1;
  LMark;
	pin_enabled[8]  = enableA2;
  LMark;
	pin_enabled[9]  = enableA3;
  LMark;
	pin_enabled[10] = enableA4;
  LMark;
	pin_enabled[11] = enableA5;
 	LMark;
}


///////////////////////////////////////////////////////////////////////////////
Loom_Digital::Loom_Digital(LoomManager* manager, JsonArrayConst p)
	: Loom_Digital(manager, EXPAND_ARRAY(p, 12) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Digital::add_config(JsonObject json)
{
  LMark;
	// add_config_aux(json, module_name,
	// 	module_name,
	// 	pin_enabled[0], pin_enabled[1], pin_enabled[2], pin_enabled[3], pin_enabled[4], pin_enabled[5],
	// 	pin_enabled[6], pin_enabled[7], pin_enabled[8], pin_enabled[9], pin_enabled[10], pin_enabled[11]
	// );
	JsonArray params = add_config_temp(json, module_name);
  LMark;
	params.add(module_name);
  LMark;
	for (int i = 0; i < 12; i++) {
   	LMark;
		params.add(pin_enabled[i]);
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Digital::print_config() const
{
  LMark;
	LoomSensor::print_config();
  LMark;

	LPrint("\tEnabled Pins        : ");
  LMark;
	// 5,6,9,10,11,12
	for (int i = 0; i < 6; i++) {
   	LMark;
		if (pin_enabled[i]) {
    	LMark;
			LPrint(pin_nums[i], ", ");
   		LMark;
		}
	}
	// A0-A5
	for (int i = 0; i < 6; i++) {
   	LMark;
		if (pin_enabled[i+6]) {
    	LMark;
			LPrint("A", i, ", ");
   		LMark;
		}
	}

	LPrintln();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Digital::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Measurements:");
  LMark;
	// 5,6,9,10,11,12
	for (int i = 0; i < 6; i++) {
   	LMark;
		if (pin_enabled[i]) {
    	LMark;
			LPrintln("\t", pin_nums[i], ": ", digital_vals[i]);
   		LMark;
		}
	}
	// A0-A5
	for (int i = 0; i < 6; i++) {
   	LMark;
		if (pin_enabled[i+6]) {
    	LMark;
			LPrintln("\t", "A", i, ": ", digital_vals[i+6]);
   		LMark;
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
void Loom_Digital::measure()
{
  LMark;
	for (int i = 0; i < DIGITAL_COUNT; i++) {
   	LMark;
		digital_vals[i] = get_digital_val(pin_nums[i]);
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Digital::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;
	char buf[3];
  LMark;

	// 5,6,9,10,11,12
	for (auto i = 0; i < 6; i++) {
   	LMark;
		if (pin_enabled[i]) {
    	LMark;
			sprintf(buf, "%d", pin_nums[i]);
    	LMark;
			data[buf] = digital_vals[i];
   		LMark;
		}
	}
	// A0-A5
	for (auto i = 0; i < 6; i++) {
   	LMark;
		if (pin_enabled[i+6]) {
    	LMark;
			sprintf(buf, "%s%d", "A", i);
    	LMark;
			data[buf] = digital_vals[i];
   		LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Digital::diagnose(bool& result){
  LMark;
	// implement here
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Digital::get_digital_val(const uint8_t pin) const
{
  LMark;
	if (pin_enabled[ pin_to_index(pin) ]) {
   	LMark;
		pinMode(pin, INPUT_PULLUP);
   	LMark;
		return digitalRead(pin);
	} else {
		LMark;
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Digital::set_digital_val(const uint8_t pin, const bool state)
{
  LMark;
	if (pin_enabled[ pin_to_index(pin) ]) {
   	LMark;
		pinMode(pin, OUTPUT);
   	LMark;
		digitalWrite(pin, (state) ? HIGH : LOW );
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Digital::get_pin_enabled(const uint8_t pin) const
{
  LMark;
	return pin_enabled[ pin_to_index(pin) ];
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Digital::set_pin_enabled(const uint8_t pin, const bool e)
{
  LMark;
	pin_enabled[ pin_to_index(pin) ] = e;
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
uint8_t Loom_Digital::pin_to_index(const uint8_t pin) const
{
  LMark;
	for (int i = 0; i < DIGITAL_COUNT; i++) {
   	LMark;
		if (pin_nums[i] == pin) return i;
  	LMark;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
