
#ifndef LOOM_DIGITAL_h
#define LOOM_DIGITAL_h

#include "Loom_Sensor.h"


#define DIGITAL_COUNT 12

// Available digital pins 5, 6, 9, 10, 11, 12, A0(14), A1(15), A2(16), A3(17), A4(18), A5(19)

class Loom_Digital : public LoomSensor
{

protected:

	/// Array of which pins are enabled
	bool	pin_enabled[DIGITAL_COUNT];

	/// Array of last read digital values
	bool	digital_vals[DIGITAL_COUNT];

public:
	// --- CONSTRUCTOR ---
	Loom_Digital(	char*   module_name 			= "Digital", 
					char*   sensor_description 		= "Digital Values",

					bool    enable5 				= false,
					bool    enable6 				= false,
					bool    enable9 				= false,
					bool    enable10 				= true,
					bool    enable11 				= false,
					bool    enable12 				= false,

					bool    enableA0 				= false,
					bool    enableA1 				= false,
					bool    enableA2 				= false,
					bool    enableA3 				= false,
					bool    enableA4 				= false,
					bool    enableA5 				= false
				);

	// --- DESTRUCTOR ---
	virtual ~Loom_Digital();

	void print_config();
	void print_measurements();
	void measure();
	void package(OSCBundle& bndl, char* suffix="");
	bool message_route(OSCMessage& msg, int address_offset) {}

	bool get_digital_val(uint8_t pin);
	void set_digital_val(uint8_t pin, bool state);

	bool get_pin_enabled(uint8_t pin);
	void set_pin_enabled(uint8_t pin, bool e);

private:

	/// Used to convert pin number to index in member arrays
	static uint8_t pin_nums[DIGITAL_COUNT];

	uint8_t pin_to_index(uint8_t pin);

};



#endif
